#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__
#include "simpleserver.hpp"
#include "simpleweb.hpp"
#include <unistd.h> //write read
#include <fcntl.h>
#include <errno.h>
#include "threadpool.hpp"
#include "eventhandler.hpp"
#ifdef IS_MACOS
#include "kqueuepoller.hpp"
#elif IS_LINUX
#include "epoller.hpp"
#endif

namespace HDE
{
    class WebServer : public SimpleServer
    {
    private:
        int thread_count;
        ThreadPool *pool;
        int server_sock;
        int timeout;
        SimplePoller *poller; //封装IO复用，实现跨平台

        std::string target_url;
        void accepter();
        void handler();
        void responder();

    public:
        WebServer(int port = 80);
        ~WebServer();
        void launch();
    };
}

#endif // __WEBSERVER_H__