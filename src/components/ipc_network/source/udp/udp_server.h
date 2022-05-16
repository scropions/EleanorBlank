#pragma once

#include <components/ipc_network/source/udp/udp_socket.h>

namespace ipc_network
{

namespace udp
{

class UdpServer final : public UdpSocket
{
    using ReceiveFromCallback = std::function<void(const boost::asio::ip::udp::endpoint&, const char*, std::size_t)>;

public:
    explicit UdpServer(
        boost::asio::io_context& context,
        std::string_view network_interface,
        std::uint16_t port,
        std::size_t recv_buffer_size);

    ~UdpServer();

    void set_receive_from_callback(ReceiveFromCallback&& callback) noexcept;

private:
    void async_receive_from() noexcept;
    void handle_receive_from(const boost::system::error_code& error, std::size_t bytes_transferred) noexcept;

private:
    ReceiveFromCallback _receive_from_callback;
    std::string _receive_buffer;
    boost::asio::ip::udp::endpoint _sender_endpoint;
};

}

}
