#include "simplepoller.hpp"

HDE::SimplePoller::SimplePoller(int socket, bool nblock, bool edg) : server_socket(socket), nonblock(nblock), edge_triggered(edg)
{
    flag_socket = fcntl(socket, F_GETFL, 0);
    if (nonblock) //设置非阻塞socket read和write等不会阻塞
    {
        flag_socket = flag_socket | O_NONBLOCK;
        fcntl(socket, F_SETFL, flag_socket);
    }
}
HDE::SimplePoller::~SimplePoller()
{
}