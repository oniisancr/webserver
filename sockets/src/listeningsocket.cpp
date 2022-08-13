#include "listeningsocket.hpp"

HDE::ListeningSocket::ListeningSocket(int domain, int type, int protocol,
                                      int port, u_long interface, int bklog) : BindingSocket(domain, type, protocol, port, interface)
{
    backlog = bklog;
    start_listening();
    test_connection(listening);

}

void HDE::ListeningSocket::start_listening()
{
    listening=listen(get_sock(),backlog);
}
