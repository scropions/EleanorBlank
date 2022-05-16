#include "pch.h"
#include "tcp_socket.h"
#include <components/data_models/source/ipc_host/packet_header.h>

namespace ipc_network
{

namespace tcp
{

TcpSocket::TcpSocket(boost::asio::io_context& context, const config::KeepAliveData& keep_alive_data) :
    _socket(context),
    _keep_alive_data{ keep_alive_data }
{
    LOG_TRACE << "TcpSocket created: " << std::hex << this << ".";
}

TcpSocket::TcpSocket(
    boost::asio::io_context& context,
    std::string_view network_interface,
    const config::KeepAliveData& keep_alive_data) :
    _socket(context, boost::asio::ip::tcp::endpoint(
                boost::asio::ip::make_address(network_interface), common::consts::any_port::value)),
    _keep_alive_data{keep_alive_data}
{
    LOG_TRACE << "TcpSocket created: " << std::hex << this << ".";
}

TcpSocket::~TcpSocket() noexcept
{
    _read_callback = nullptr;
    _write_callback = nullptr;
    _connect_callback = nullptr;
    _connect_failed_callback = nullptr;
    _disconnect_callback = nullptr;
    close();
}

StatusCode TcpSocket::set_keep_alive() noexcept
{
    try
    {
        if (!_keep_alive_data.get_enabled())
        {
            return StatusCode::GoodNothingTodo;
        }

        boost::system::error_code error;
        boost::asio::socket_base::keep_alive opt_keep_alive(true);
        _socket.set_option(opt_keep_alive, error);

        if (error)
        {
            LOG_ERROR << "TcpSocket: failed to set keep alive: " << error.message() << ".";
            return StatusCode::Bad;
        }

        auto timeout = _keep_alive_data.get_timeout();
        auto cnt = _keep_alive_data.get_cnt();
        auto intverval = _keep_alive_data.get_intverval();

#ifndef SOL_TCP
#define SOL_TCP IPPROTO_TCP
#endif
        auto s = _socket.native_handle();
        boost::asio::detail::socket_ops::state_type state = 0;
        boost::asio::detail::socket_ops::setsockopt(
            s, state, SOL_TCP, TCP_KEEPIDLE, reinterpret_cast<const char*>(&timeout), sizeof(timeout), error);

        if (error)
        {
            LOG_ERROR << "TcpSocket failed to set TCP_KEEPIDLE: " << error.message() << ".";
            return StatusCode::Bad;
        }

        boost::asio::detail::socket_ops::setsockopt(
            s, state, SOL_TCP, TCP_KEEPCNT, reinterpret_cast<const char*>(&cnt), sizeof(cnt), error);

        if (error)
        {
            LOG_ERROR << "TcpSocket failed to set TCP_KEEPCNT: " << error.message() << ".";
            return StatusCode::Bad;
        }

        boost::asio::detail::socket_ops::setsockopt(
            s, state, SOL_TCP, TCP_KEEPINTVL, reinterpret_cast<const char*>(&intverval), sizeof(intverval), error);

        if (error)
        {
            LOG_ERROR << "TcpSocket failed to set TCP_KEEPINTVL: " << error.message() << ".";
            return StatusCode::Bad;
        }

#undef SOL_TCP


    }
    catch (...)
    {
        LOG_ERROR << "TcpSocket failed to set keep alive data: " << utils::what_s();
    }

    return StatusCode::Good;
}

void TcpSocket::set_read_callback(ReadCallback&& callback) noexcept
{
    try
    {
        _read_callback = std::move(callback);
    }
    catch (...)
    {
        LOG_ERROR << "TcpSocket failed to set read callback: " << utils::what_s();
    }
}

void TcpSocket::set_write_callback(WriteCallback&& callback) noexcept
{
    try
    {
        _write_callback = std::move(callback);
    }
    catch (...)
    {
        LOG_ERROR << "TcpSocket failed to set write callback: " << utils::what_s();
    }
}

void TcpSocket::set_disconnect_callback(DisconnectCallback&& callback) noexcept
{
    try
    {
        _disconnect_callback = std::move(callback);
    }
    catch (...)
    {
        LOG_ERROR << "TcpSocket failed to set disconnect callback: " << utils::what_s();
    }
}

void TcpSocket::set_connect_callback(ConnectCallback&& callback) noexcept
{
    try
    {
        _connect_callback = std::move(callback);
    }
    catch (...)
    {
        LOG_ERROR << "TcpSocket failed to set connect callback: " << utils::what_s();
    }
}

void TcpSocket::set_connect_failed_callback(ConnectCallback&& callback) noexcept
{
    try
    {
        _connect_failed_callback = std::move(callback);
    }
    catch (...)
    {
        LOG_ERROR << "TcpSocket failed to set connect failed callback: " << utils::what_s();
    }
}

void TcpSocket::set_socket(boost::asio::ip::tcp::socket&& s) noexcept
{
    try
    {
        close();
        _socket = std::move(s);
    }
    catch (...)
    {
        LOG_ERROR << "TcpSocket failed to set socket: " << utils::what_s();
    }
}

boost::asio::ip::tcp::socket& TcpSocket::get_socket() noexcept
{
    return _socket;
}

void TcpSocket::async_read_header() noexcept
{
    try
    {
        _read_packet.resize(sizeof(data_models::PacketHeader));
        boost::asio::async_read(_socket,
                                boost::asio::buffer(_read_packet),
                                boost::bind(&TcpSocket::handle_read_header, this,
                                            boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    catch (...)
    {
        LOG_ERROR << "TcpSocket failed to async read header: " << utils::what_s();
    }
}

void TcpSocket::async_read_data(size_t size) noexcept
{
    auto old_size = _read_packet.size();

    try
    {
        _read_packet.resize(old_size + size);
    }
    catch (...)
    {
        LOG_ERROR << "TcpSocket failed to resize buffer: " << utils::what_s() << ".";

        async_read_header();

        return;
    }

    try
    {
        boost::asio::async_read(_socket,
                                boost::asio::buffer(_read_packet.data() + old_size, size),
                                boost::bind(&TcpSocket::handle_read_data, this,
                                            boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    catch (...)
    {
        LOG_ERROR << "TcpSocket failed to async read in read data: " << utils::what_s();
    }
}

void TcpSocket::handle_read_header(const boost::system::error_code& error, std::size_t bytes_transferred) noexcept
{
    try
    {
        if (error)
        {
            LOG_INFO << "TcpSocket failed to read header: " << error.message() << ".";
            close();
            return;
        }

        if (sizeof(data_models::PacketHeader) != bytes_transferred)
        {
            LOG_WARN << "TcpSocket received wrong header.";
            async_read_header();
            return;
        }

        auto header = reinterpret_cast<data_models::PacketHeader*>(_read_packet.data());

        if (nullptr == header)
        {
            LOG_ERROR << "TcpSocket wrong header buffer.";
            close();
            return;
        }

        const auto size = header->get_size();

        if (size > 0)
        {
            async_read_data(size);
            return;
        }

        if (_read_callback)
        {
            _read_callback(_read_packet.data(), _read_packet.size());
        }

        async_read_header();
    }
    catch (...)
    {
        LOG_ERROR << "TcpSocket failed to handle read header: " << utils::what_s();
    }
}

void TcpSocket::handle_read_data(const boost::system::error_code& error, std::size_t bytes_transferred) noexcept
{
    try
    {
        if (error)
        {
            LOG_WARN << "TcpSocket failed to read data: " << error.message() << ".";
            close();
            return;
        }

        if (_read_packet.size() - sizeof(data_models::PacketHeader) != bytes_transferred)
        {
            LOG_WARN << "TcpSocket received wrong header data.";
            async_read_header();
            return;
        }

        if (_read_callback)
        {
            _read_callback(_read_packet.data(), _read_packet.size());
        }

        async_read_header();
    }
    catch (...)
    {
        LOG_ERROR << "TcpSocket failed to handle read data: " << utils::what_s();
    }
}

void TcpSocket::async_write(const std::string& packet) noexcept
{
    bool write_in_progress = !_write_packets.empty();

    try
    {
        _write_packets.push_back(packet);
    }
    catch (...)
    {
        LOG_ERROR << "TcpSocket failed to write: " << utils::what_s() << ".";
        close();
    }

    try
    {
        if (!write_in_progress)
        {
            boost::asio::async_write(_socket,
                                     boost::asio::buffer(_write_packets.front().data(),
                                             _write_packets.front().length()),
                                     boost::bind(&TcpSocket::handle_write, this,
                                                 boost::asio::placeholders::error));
        }
    }
    catch (...)
    {
        LOG_ERROR << "TcpSocket failed to async write: " << utils::what_s();
    }
}

void TcpSocket::handle_write(const boost::system::error_code& error) noexcept
{
    if (!error)
    {
        try
        {
            _write_packets.pop_front();

            if (!_write_packets.empty())
            {
                boost::asio::async_write(_socket,
                                         boost::asio::buffer(_write_packets.front().data(),
                                                 _write_packets.front().length()),
                                         boost::bind(&TcpSocket::handle_write, this,
                                                     boost::asio::placeholders::error));
            }
        }
        catch (...)
        {
            LOG_ERROR << "TcpSocket failed to async write in handle write: " << utils::what_s();
        }
    }
    else
    {
        LOG_WARN << "TcpSocket failed to make async write: " << error.message() << ".";
        close();
    }
}

void TcpSocket::close() noexcept
{
    try
    {
        if (_socket.is_open())
        {
            boost::system::error_code error;
            _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
            _socket.close(error);
            on_disconnected();
            LOG_TRACE << "TcpSocket closed: " << std::hex << this << ".";
        }
    }
    catch (...)
    {
        LOG_ERROR << "TcpSocket failed to close socket: " << utils::what_s();
    }
}

void TcpSocket::on_disconnected() noexcept
{
    try
    {
        if (_disconnect_callback)
        {
            _disconnect_callback();
        }
    }
    catch (...)
    {
        LOG_ERROR << "TcpSocket failed to call disconnect callback: " << utils::what_s();
    }
}

}
}
