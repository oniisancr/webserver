#ifndef __TESTSERVER_H__
#define __TESTSERVER_H__

#include "simpleserver.hpp"
#include <unistd.h> //write read
#include <string>
namespace HDE
{
    class TestServer : public SimpleServer
    {
    private:
        char buffer[30000] = {0};
        int new_socket;
        sockaddr_in clnt_addr;  //用于保存客户端的信息
        void accepter();
        void handler();
        void responder();

    public:
        TestServer();
        void launch();
    };
}

#endif // __TESTSERVER_H__