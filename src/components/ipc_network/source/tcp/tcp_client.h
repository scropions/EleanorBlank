#pragma once

#include <memory>
#include <string_view>

#include <boost/asio/ip/tcp.hpp>

#include <components/ipc_network/source/tcp/tcp_socket.h>
#include <common/config_helper.h>

namespace ipc_network
{

namespace tcp
{

class TcpClient final : public TcpSocket
{
public:
    TcpClient(
        boost::asio::io_context& context,
        std::string_view network_interface,
        std::string_view host,
        std::uint16_t port,
        const config::KeepAliveData& keep_alive_data);

    ~TcpClient();

private:
    void handle_connect(const boost::system::error_code& error) noexcept;

private:
    std::unique_ptr<boost::asio::ip::tcp::resolver::results_type> _endpoints;
};

}

}
