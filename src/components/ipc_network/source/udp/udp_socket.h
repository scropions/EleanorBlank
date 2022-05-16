#pragma once

namespace ipc_network::udp
{

class UdpSocket
{
    using SendCallback = std::function<void()>;

protected:
    explicit UdpSocket(boost::asio::io_context& context, std::string_view network_interface, std::uint16_t port);
    explicit UdpSocket(boost::asio::io_context& context, std::string_view network_interface);

    virtual ~UdpSocket() noexcept;

public:
    void async_send_to(const boost::asio::ip::udp::socket::endpoint_type& destination, std::string&& packet);
    void async_send_to(const boost::asio::ip::udp::socket::endpoint_type& destination, const std::string& packet);

    boost::asio::ip::udp::socket& get_socket() noexcept;
    boost::asio::ip::udp::endpoint get_local_endpoint() const noexcept;

private:
    boost::asio::ip::udp::socket _socket;
};

}
