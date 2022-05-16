// udp_server.cpp
#include "pch.h"
#include "udp_server.h"

namespace ipc_network
{

namespace udp
{

UdpServer::UdpServer(
    boost::asio::io_context& context,
    std::string_view network_interface,
    std::uint16_t port,
    std::size_t recv_buffer_size) :
    UdpSocket(context, network_interface, port)
{
    _receive_buffer.resize(recv_buffer_size);

    LOG_TRACE << "UdpServer created: " << std::hex << this << ".";

    async_receive_from();

    LOG_TRACE << "UdpServer started: " << std::hex << this << ".";
}

UdpServer::~UdpServer()
{
    LOG_TRACE << "UdpServer destroyed: " << std::hex << this << ".";
}

void UdpServer::set_receive_from_callback(ReceiveFromCallback&& callback) noexcept
{
    _receive_from_callback = std::move(callback);
}

void UdpServer::async_receive_from() noexcept
{
    get_socket().async_receive_from(
        boost::asio::buffer(_receive_buffer),
        _sender_endpoint,
        boost::bind(&UdpServer::handle_receive_from, this,
                    boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void UdpServer::handle_receive_from(const boost::system::error_code& error, std::size_t bytes_transferred) noexcept
{
    if (!error)
    {
        if (_receive_from_callback)
        {
            _receive_from_callback(_sender_endpoint, _receive_buffer.data(), bytes_transferred);
        }

        async_receive_from();
    }
    else
    {
        LOG_WARN << "UdpServer: failed to make async receive: " << error.message() << "'.";
    }
}

}

}
