#ifndef __BINDINGSOCKET_H__
#define __BINDINGSOCKET_H__

#include "simplesocket.hpp"

namespace HDE
{
    class BindingSocket : public SimpleSocket
    {
    public:
        BindingSocket(int domain, int type, int protocol,int port, u_long interface);
        int connect_to_network(int sock, sockaddr_in addr) override;
    };
}

#endif // __BINDINGSOCKET_H__