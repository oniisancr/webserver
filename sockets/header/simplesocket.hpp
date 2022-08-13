#ifndef __SIMPLESOCKET_H__
#define __SIMPLESOCKET_H__

#include <sys/socket.h>
#include <arpa/inet.h> //sockaddr_in
#include <iostream>

namespace HDE
{
    class SimpleSocket
    {
    private:
        sockaddr_in address;
        int sock;
        int connection;

    public:
        SimpleSocket(int domain, int type, int protocol, int port, u_long interface);
        // bind(for server) or connect(for client)
        // 不同sock有不同的行为
        virtual int connect_to_network(int sock, sockaddr_in addr) = 0;
        void test_connection(int);
        sockaddr_in get_address();
        int get_sock();
        int get_connection();
        void set_connection(int);
    };
}

#endif // __SIMPLESOCKET_H__