#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__
#include "simpleserver.hpp"
#include "simpleweb.hpp"
#include <unistd.h> //write read
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

#define BUFSZ 5
#define EPOLLSZ 50

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
        int epfd;
        epoll_event *epoll_evens;
        epoll_event event;
        int event_cnt;
        int timeout;

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