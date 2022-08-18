#ifndef __SIMPLEPOLLER_H__
#define __SIMPLEPOLLER_H__
#include <string>
#include <fcntl.h>
#include <vector>
#include <arpa/inet.h> //sockaddr_in

namespace HDE
{
    class SimplePoller
    {
    protected:
        int server_socket;
        bool nonblock;       //是否阻塞
        bool edge_triggered; //是否边缘触发
        int flag_socket;
    public:
        // 触发的events
        std::vector<int> eventsVec;
        SimplePoller(int sv, bool block, bool edg);
        // add new socket
        virtual void add_socket(int) = 0;
        // delete socket
        virtual void free_socket(int) = 0;
        virtual ~SimplePoller();
        // watch/wait events
        virtual int watch(int) = 0;
    };
}

#endif // __SIMPLEPOLLER_H__