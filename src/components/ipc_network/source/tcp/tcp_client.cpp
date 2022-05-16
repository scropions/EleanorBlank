#include "pch.h"
#include "tcp_client.h"

namespace ipc_network
{

namespace tcp
{

TcpClient::TcpClient(
    boost::asio::io_context& context,
    std::string_view network_interface,
    std::string_view host,
    std::uint16_t port,
    const config::KeepAliveData& keep_alive_data) :
    TcpSocket(context, network_interface, keep_alive_data),
    _endpoints(std::make_unique<boost::asio::ip::tcp::resolver::results_type>(
                   boost::asio::ip::tcp::resolver(context).resolve(host, std::to_string(port))))
{
    LOG_TRACE << "TcpClient created: " << std::hex << this << ".";

    if (_endpoints)
    {
        LOG_TRACE << "TcpClient connecting to [count, name, address, port]: " << _endpoints->size() << ", "
                  << _endpoints->begin()->host_name() << ", "
                  << _endpoints->begin()->endpoint().address() << ", "
                  << _endpoints->begin()->endpoint().port();
        boost::asio::async_connect(get_socket(), *_endpoints,
                                   boost::bind(&TcpClient::handle_connect, this,
                                               boost::asio::placeholders::error));
    }

    LOG_TRACE << "TcpClient started: " << std::hex << this << ".";
}

TcpClient::~TcpClient()
{
    LOG_TRACE << "TcpClient destroyed: " << std::hex << this << ".";
}

void TcpClient::handle_connect(const boost::system::error_code& error) noexcept
{
    try
    {
        if (!error)
        {
            CHECK_SC(set_keep_alive());
            if (_connect_callback)
            {
                _connect_callback();
            }
            async_read_header();
        }
        else
        {
            LOG_WARN << "TcpClient: failed to make async connect: " << error.message() << "'.";
            if (_connect_failed_callback)
            {
                _connect_failed_callback();
            }
        }
    }
    catch (...)
    {
        LOG_ERROR << "TcpClient failed to call callbacks: " << utils::what_s();
    }
}

}

}
