#ifndef __EPOLLER_H__
#define __EPOLLER_H__
#include "simplepoller.hpp"
#include <iostream>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

// reference: https://dev.to/frevib/a-tcp-server-with-kqueue-527


#define EPOLLSZ 10

namespace HDE
{
    class Epoller : public SimplePoller
    {
    private:
        int timeout;
        int epfd;
        epoll_event *epoll_evens;
        epoll_event event;
        int flag_event;

    public:
        Epoller(int,bool,bool);
        ~Epoller();
        void add_socket(int);
        // delete socket
        void free_socket(int);
        int watch(int);
    };
}

#endif // __EPOLLER_H__