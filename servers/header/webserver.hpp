#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__
#include "simpleserver.hpp"
#include "simpleweb.hpp"
#include <unistd.h> //write read

#define BUFSZ 30000

namespace HDE
{
    class WebServer : public SimpleServer
    {
    private:
        char buffer[BUFSZ] = {0};
        int server_sock;
        int new_socket;
        sockaddr_in clnt_addr; //用于保存客户端的信息
        int fd_max;
        fd_set reads, copy_reads;
        timeval timeout;
        std::string target_url;
        void accepter();
        void handler();
        void responder();

    public:
        WebServer();
        void launch();
    };
}

#endif // __WEBSERVER_H__