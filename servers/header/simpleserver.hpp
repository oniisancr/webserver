#ifndef __SIMPLESERVER_H__
#define __SIMPLESERVER_H__
#include "listeningsocket.hpp"

namespace HDE
{
    class SimpleServer
    {
    private:
        ListeningSocket *sock_ptr;
        virtual void accepter() = 0;

    public:
        SimpleServer(int domain, int type, int protocol, int port, u_long interface, int bklog);
        virtual void launch() = 0;
        ListeningSocket *get_socket();
        virtual ~SimpleServer() { delete sock_ptr; }
    };
}
#endif // __SIMPLESERVER_H__