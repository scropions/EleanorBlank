#pragma once

#include <deque>
#include <string>

#include <boost/asio/ip/tcp.hpp>

#include <common/config_helper.h>

namespace ipc_network
{

namespace tcp
{

class TcpSocket
{
    using ReadCallback = std::function<void(const char* data, std::size_t size)>;
    using WriteCallback = std::function<void()>;
    using ConnectCallback = std::function<void()>;
    using DisconnectCallback = std::function<void()>;

protected:
    explicit TcpSocket(boost::asio::io_context& context, const config::KeepAliveData& keep_alive_data);
    explicit TcpSocket(
        boost::asio::io_context& context,
        std::string_view network_interface,
        const config::KeepAliveData& keep_alive_data);

    virtual ~TcpSocket() noexcept;

public:
    void set_read_callback(ReadCallback&& callback) noexcept;
    void set_write_callback(WriteCallback&& callback) noexcept;
    void set_disconnect_callback(DisconnectCallback&& callback) noexcept;
    void set_connect_callback(ConnectCallback&& callback) noexcept;
    void set_connect_failed_callback(ConnectCallback&& callback) noexcept;

    void set_socket(boost::asio::ip::tcp::socket&& s) noexcept;
    boost::asio::ip::tcp::socket& get_socket() noexcept;

    void async_write(const std::string& packet) noexcept;
    void close() noexcept;

protected:
    StatusCode set_keep_alive() noexcept;
    void async_read_header() noexcept;

private:
    void async_read_data(size_t size) noexcept;

    void handle_read_header(const boost::system::error_code& error, std::size_t bytes_transferred) noexcept;
    void handle_read_data(const boost::system::error_code& error, std::size_t bytes_transferred) noexcept;
    void handle_write(const boost::system::error_code& error) noexcept;

private:
    void on_disconnected() noexcept;

protected:
    ReadCallback _read_callback;
    WriteCallback _write_callback;
    ConnectCallback _connect_callback;
    ConnectCallback _connect_failed_callback;
    DisconnectCallback _disconnect_callback;

private:
    boost::asio::ip::tcp::socket _socket;
    config::KeepAliveData _keep_alive_data;
    std::string _read_packet;

    std::deque<std::string> _write_packets;
};

}

}
