#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__
#include "simpleserver.hpp"
#include "simpleweb.hpp"
#include <unistd.h> //write read
#include <fcntl.h>
#include <errno.h>
#ifdef IS_MACOS
    #include "kqueuepoller.hpp"
#elif IS_LINUX
    #include "epoller.hpp"
#endif


#define BUFSZ 2048

namespace HDE
{
    class WebServer : public SimpleServer
    {
    private:
        char buffer[BUFSZ] = {0};
        int server_sock;
        int clnt_socket;
        std::string clnt_data;
        sockaddr_in clnt_addr; //用于保存客户端的信息
        int timeout;
        SimplePoller *poller; //封装IO复用，实现跨平台

        std::string target_url;
        void accepter();
        void handler();
        void responder();

    public:
        WebServer();
        ~WebServer();
        void launch();
    };
}

#endif // __WEBSERVER_H__