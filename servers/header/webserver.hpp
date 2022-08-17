#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__
#include "simpleserver.hpp"
#include "simpleweb.hpp"
#include <new>
#include <unistd.h> //write read
#include <fcntl.h>
#include <sys/event.h> //kevent
#include <sys/types.h>
#include <string>

#define BUFSZ 2048
#define KQUEUESZ 10

namespace HDE
{
    class WebServer : public SimpleServer
    {
    private:
        char buffer[BUFSZ] = {0};
        std::string clnt_data;
        int server_sock;
        int clnt_socket;
        sockaddr_in clnt_addr; //用于保存客户端的信息
        int kq;
        struct kevent *changelist; //每次注册事件
        struct kevent *events;     //每次返回事件
        timespec timeout;
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