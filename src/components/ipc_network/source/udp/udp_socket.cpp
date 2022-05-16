#include "pch.h"
#include "udp_socket.h"

namespace ipc_network::udp
{

UdpSocket::UdpSocket(boost::asio::io_context& context, std::string_view network_interface, std::uint16_t port)
    : _socket(context,
              boost::asio::ip::udp::endpoint(boost::asio::ip::make_address(network_interface), port))
{
    LOG_TRACE << "UdpSocket created: " << std::hex << this << ".";
}

UdpSocket::UdpSocket(boost::asio::io_context& context, std::string_view network_interface)
    : _socket(context,
              boost::asio::ip::udp::endpoint(boost::asio::ip::make_address(network_interface), 0))
{
    LOG_TRACE << "UdpSocket created: " << std::hex << this << ".";
}

UdpSocket::~UdpSocket() noexcept
{
    if (_socket.is_open())
    {
        boost::system::error_code error;
        _socket.shutdown(boost::asio::ip::udp::socket::shutdown_both, error);
        _socket.close(error);
    }

    LOG_TRACE << "UdpSocket closed: " << std::hex << this << ".";
}

void UdpSocket::async_send_to(const boost::asio::ip::udp::socket::endpoint_type& destination,
                              std::string&& packet)
{
    auto packet_ptr = std::make_unique<std::string>(std::move(packet));
    auto buffer = boost::asio::buffer(packet_ptr->data(), packet_ptr->size());
    _socket.async_send_to(buffer, destination,
                          [this, p = std::move(packet_ptr)](boost::system::error_code ec, std::size_t /*bytes*/)
    {
        if (ec)
        {
            LOG_WARN << "UdpSocket: failed to send: " << ec.message() << ".";
        }
    });
}

void UdpSocket::async_send_to(const boost::asio::ip::udp::socket::endpoint_type& destination,
                              const std::string& packet)
{
    async_send_to(destination, std::string(packet));
}

boost::asio::ip::udp::socket& UdpSocket::get_socket() noexcept
{
    return _socket;
}

boost::asio::ip::udp::endpoint UdpSocket::get_local_endpoint() const noexcept
{
    boost::system::error_code ec;
    auto ep = _socket.local_endpoint(ec);
    return ec ? boost::asio::ip::udp::endpoint{} :
           ep;
}

}
