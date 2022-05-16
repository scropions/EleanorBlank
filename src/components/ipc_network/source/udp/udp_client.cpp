// udp_client.cpp
#include "pch.h"
#include "udp_client.h"

namespace ipc_network
{

namespace udp
{

UdpClient::UdpClient(boost::asio::io_context& context, std::string_view network_interface) :
    UdpSocket(context, network_interface)
{
    LOG_TRACE << "UdpClient: created.";
}

}

}
