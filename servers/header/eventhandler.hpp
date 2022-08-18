#ifndef __EVENTHANDLER_H__
#define __EVENTHANDLER_H__
#include <string>
#include <errno.h>
#include <unistd.h> //write read
#include "eventhandler.hpp"
#include "simpleweb.hpp"
#include "simplepoller.hpp"
#define BUFSZ 2048
namespace HDE
{
    class EventHandler
    {
    private:
        char buffer[BUFSZ] = {0};
        SimplePoller *poller;
        int clnt_socket;
        std::string clnt_data;
        std::string target_url;
        void handler();
        void responder();

    public:
        EventHandler(SimplePoller *, int);
        ~EventHandler();
        void run();
    };
}

#endif // __EVENTHANDLER_H__