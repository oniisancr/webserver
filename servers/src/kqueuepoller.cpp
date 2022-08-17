#include "kqueuepoller.hpp"

HDE::KququePoller::KququePoller(int sv, bool nblock, bool edg) : HDE::SimplePoller(sv, nblock, edg)
{
    initate();
}
HDE::KququePoller::~KququePoller()
{
    delete[] changelist;
    delete[] events;
}
void HDE::KququePoller::initate()
{
    kq = kqueue();
    // 初始化事件列表
    changelist = new struct kevent[KQUEUESZ];
    events = new struct kevent[KQUEUESZ];
    //注册server_socket监听事件  边缘触发EV_CLEAR
    flag_event = EV_ADD | EV_ENABLE | EV_CLEAR;
    if (!edge_triggered)
        flag_event = EV_ADD | EV_ENABLE;
    EV_SET(changelist, server_socket, EVFILT_READ, flag_event, 0, 0, 0);
    if (kevent(kq, changelist, 1, NULL, 0, NULL) == -1)
    {
        perror("kevent error");
        exit(-1);
    }
}
void HDE::KququePoller::add_socket(int skt)
{
    sockaddr_in clnt_addr;
    //设置是否阻塞
    fcntl(skt, F_SETFL, flag_socket);
    //注册该socket  每次注册1个socket
    EV_SET(changelist, skt, EVFILT_READ, flag_event, 0, 0, 0);
    if (kevent(kq, changelist, 1, NULL, 0, NULL) == -1)
    {
        perror("kevent add error");
    }
}
void HDE::KququePoller::free_socket(int skt)
{
}
int HDE::KququePoller::watch(int time_out = -1)
{
    eventsVec.clear();
    timeout.tv_sec = time_out;
    timeout.tv_nsec = 0;
    int new_events = 0;
    if (time_out == -1)
        new_events = kevent(kq, nullptr, 0, events, KQUEUESZ, nullptr);
    else
        new_events = kevent(kq, nullptr, 0, events, KQUEUESZ, &timeout);
    for (int i = 0; i < new_events; i++)
    {
        eventsVec.push_back(events[i].ident);
    }
    return new_events;
}
