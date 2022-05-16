// tcp_server.cpp
#include "pch.h"
#include "tcp_server.h"
#include <common/config_helper.h>

namespace ipc_network
{

namespace tcp
{

TcpServer::TcpServer(
    boost::asio::io_context& context,
    std::string_view network_interface,
    std::uint16_t port,
    const config::KeepAliveData& keep_alive_data,
    const FilterCallback& filter) :
    TcpSocket(context, keep_alive_data),
    _acceptor(std::make_unique<boost::asio::ip::tcp::acceptor>(
                  context, boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(network_interface), port))),
    _filter(filter)
{
    CHECK_SC_THROW(async_accept());

    LOG_TRACE << "TcpServer created: " << std::hex << this << ".";
}

StatusCode TcpServer::async_accept() noexcept
{
    CHECK_POINTER_R(_acceptor);
    _acceptor->async_accept(
        [this](const boost::system::error_code& error, boost::asio::ip::tcp::socket socket)
    {
        if (error)
        {
            LOG_WARN << "TcpServer failed to accept: " << error.message() << "'.";
        }

        if (_filter && !_filter(socket))
        {
            close(socket);
            CHECK_SC(async_accept());
        }
        else
        {
            set_socket(std::move(socket));
            CHECK_SC(set_keep_alive());
            async_read_header();
        }
    });
    return StatusCode::Good;
}

void TcpServer::close(boost::asio::ip::tcp::socket& socket) const noexcept
{
    if (socket.is_open())
    {
        boost::system::error_code error;
        socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
        socket.close(error);
    }
}

}

}
