#pragma once

#include <components/ipc_network/source/udp/udp_socket.h>

namespace ipc_network
{

namespace udp
{

class UdpClient final : public UdpSocket
{
public:
    explicit UdpClient(boost::asio::io_context& context, std::string_view network_interface);
};

}

}
