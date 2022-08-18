#ifndef __KQUEUEPOLLER_H__
#define __KQUEUEPOLLER_H__
#include <cstdio>      //perror
#include <sys/event.h> //kevent
#include <unistd.h>    //write read
#include <sys/types.h>
#include <new>
#define KQUEUESZ 10

#include "simplepoller.hpp"
namespace HDE
{
    class KququePoller : public SimplePoller
    {
    private:
        int kq;
        struct kevent *changelist; //每次注册事件
        struct kevent *events;     //每次返回事件
        timespec timeout;
        int flag_event; //用于设置event flag
        void initate();

    public:
        KququePoller(int sv, bool nbook, bool edg);
        ~KququePoller();
        void add_socket(int);
        // delete socket
        void free_socket(int);
        int watch(int);
    };
}
#endif // __KQUEUEPOLLER_H__