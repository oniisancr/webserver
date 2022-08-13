#ifndef __LISTENINGSOCKET_H__
#define __LISTENINGSOCKET_H__

#include "bindingsocket.hpp"

namespace HDE
{
    class ListeningSocket : public BindingSocket
    {
    private:
        int backlog;    //监听的最大队列长度
        int listening;
    public:
        ListeningSocket(int domain, int type, int protocol,int port, u_long interface, int bklog);
        void start_listening();
    };
}

#endif // __LISTENINGSOCKET_H__