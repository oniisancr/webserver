#include "connectingsocket.hpp"

HDE::ConnectingSocket::ConnectingSocket(int domain, int type, int protocol,
                                  int port, u_long interface) : SimpleSocket(domain, type, protocol, port, interface)
{
    set_connection(connect_to_network(get_sock(), get_address()));
    test_connection(get_connection());
}

int HDE::ConnectingSocket::connect_to_network(int sock, sockaddr_in address)
{
    return connect(sock, (sockaddr *)&address, sizeof(address));
}
