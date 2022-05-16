#pragma once
#include <components/ipc_network/source/tcp/tcp_socket.h>

namespace ipc_network
{

namespace tcp
{

class TcpServer final : public TcpSocket
{
    using FilterCallback = std::function<bool(const boost::asio::ip::tcp::socket& socket)>;

public:
    TcpServer(
        boost::asio::io_context& context,
        std::string_view network_interface,
        std::uint16_t port,
        const config::KeepAliveData& keep_alive_data,
        const FilterCallback& filter = nullptr);

    StatusCode async_accept() noexcept;

private:
    void close(boost::asio::ip::tcp::socket& socket) const noexcept;

private:
    std::unique_ptr<boost::asio::ip::tcp::acceptor> _acceptor;
    FilterCallback _filter;
};

}

}
