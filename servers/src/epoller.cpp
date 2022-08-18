#include "epoller.hpp"

HDE::Epoller::Epoller(int sv, bool nblock, bool edg) : HDE::SimplePoller(sv, nblock, edg)
{
    flag_socket = fcntl(server_socket, F_GETFL, 0);
    if (nblock) //设置非阻塞
        flag_socket = flag_socket | O_NONBLOCK;
    fcntl(server_socket, F_SETFL, flag_socket);
    timeout = -1;
    // initate epoll
    epfd = epoll_create(EPOLLSZ);
    epoll_evens = new epoll_event[EPOLLSZ];
    flag_event = EPOLLIN;
    if (edg) // edge-triggered
        flag_event = EPOLLIN | EPOLLET;
    event.events = flag_event;
    event.data.fd = server_socket;
    epoll_ctl(epfd, EPOLL_CTL_ADD, server_socket, &event);
}
HDE::Epoller::~Epoller()
{
    delete[] epoll_evens;
}
void HDE::Epoller::add_socket(int skt)
{
    //设置是否阻塞
    fcntl(skt, F_SETFL, flag_socket);
    // 注册事件
    event.events = flag_event;
    event.data.fd = skt;
    if (-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, skt, &event))
    {
        perror("epoll add error");
    }
}
// delete socket
void HDE::Epoller::free_socket(int skt)
{

    epoll_ctl(epfd, EPOLL_CTL_DEL, skt, nullptr);
}
// time_out -1 means waitinng until client socke be accepted
int HDE::Epoller::watch(int time_out = -1)
{
    timeout = time_out;
    eventsVec.clear();
    int event_cnt = 0;
    if ((event_cnt = epoll_wait(epfd, epoll_evens, EPOLLSZ, timeout)) == -1)
    {
        std::cout << "epoll_wait error...try again" << std::endl;
    }
    for (int i = 0; i < event_cnt; i++)
    {
        eventsVec.push_back(epoll_evens[i].data.fd);
    }
    return event_cnt;
}