#ifndef __CONNECTINGSOCKET_H__
#define __CONNECTINGSOCKET_H__
#include "simplesocket.hpp"

namespace HDE
{
    class ConnectingSocket : public SimpleSocket
    {
    public:
        ConnectingSocket(int domain, int type, int protocol, int port, u_long interface);
        int connect_to_network(int sock, sockaddr_in addr) override;
    };

}

#endif // __CONNECTINGSOCKET_H__