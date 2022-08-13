#include "simpleserver.hpp"

HDE::SimpleServer::SimpleServer(int domain, int type, int protocol, int port, u_long interface, int bklog)
{
    sock_ptr = new ListeningSocket(domain, type, protocol, port, interface, bklog);
}

HDE::ListeningSocket *HDE::SimpleServer::get_socket()
{
    return sock_ptr;
}
