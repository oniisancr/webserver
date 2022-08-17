#include "webserver.hpp"

// reference: https://dev.to/frevib/a-tcp-server-with-kqueue-527

HDE::WebServer::WebServer() : SimpleServer(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10)
{
    // timeout.tv_sec = 0;
    // timeout.tv_nsec = 0;
    server_sock = get_socket()->get_sock();
    kq = kqueue();
    // 设置非阻塞socket read和write等不会阻塞
    int flag = fcntl(server_sock, F_GETFL, 0);
    fcntl(server_sock, F_SETFL, flag | O_NONBLOCK);
    // 初始化事件列表
    changelist = new struct kevent[KQUEUESZ];
    events = new struct kevent[KQUEUESZ];
    //注册server_socket监听事件  边缘触发EV_CLEAR
    EV_SET(changelist, server_sock, EVFILT_READ, EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0, 0);
    if (kevent(kq, changelist, 1, NULL, 0, NULL) == -1)
    {
        perror("kevent error");
    }
}
HDE::WebServer::~WebServer()
{
    delete[] changelist;
    delete[] events;
}

void HDE::WebServer::accepter()
{
    int addrlen = sizeof(clnt_addr);
    clnt_socket = accept(server_sock, (sockaddr *)&clnt_addr, (socklen_t *)&addrlen);
    if (clnt_socket == -1)
    {
        perror("Accept socket error");
        return;
    }
    // 设置非阻塞socket
    int flag = fcntl(clnt_socket, F_GETFL, 0);
    fcntl(clnt_socket, F_SETFL, flag | O_NONBLOCK);
    //注册该socket  每次注册1个socket
    EV_SET(changelist, clnt_socket, EVFILT_READ, EV_ADD | EV_ENABLE | EV_CLEAR, 0, 0, 0);
    if (kevent(kq, changelist, 1, NULL, 0, NULL) == -1)
    {
        perror("kevent add error");
    }
}
void HDE::WebServer::handler()
{
    std::cout << "client addr: " << inet_ntoa(clnt_addr.sin_addr) << ":" << ntohs(clnt_addr.sin_port);
    string url = "";
    int cnt = 0;
    // 取出需要的页面 在第一个空格到第二个空格之间 仅实现GET请求
    for (int i = 0; i < clnt_data.size(); i++)
    {
        char c = clnt_data[i];
        if (cnt == 1 && c != ' ' && c != '/')
            url += c;
        if (c == ' ')
            cnt++;
        if (cnt > 1)
            break;
    }
    if (url.empty())
        url = "index.html";
    std::cout << " --> " << url << std::endl;
    target_url = url;
}
void HDE::WebServer::responder()
{
    SimpleWeb web(target_url);
    write(clnt_socket, string(web).c_str(), string(web).size());
}
void HDE::WebServer::launch()
{
    int new_events = 0;
    while (true)
    {
        std::cout << "=====WAITING===== " << std::endl;
        if ((new_events = kevent(kq, nullptr, 0, events, KQUEUESZ, nullptr)) == -1)
        {
            std::cout << "kevevt error...try again" << std::endl;
            continue;
        }
        // time out
        if (new_events == 0)
            continue;

        for (int i = 0; i < new_events; i++)
        {
            int event_fd = events[i].ident;
            // 断开链接 客户端发送EOF后，kqueue会自动删除相关事件
            if (events[i].flags & EV_EOF)
            {
                close(event_fd);
                continue;
            }
            if (event_fd == server_sock) //有新的连接请求
            {
                accepter();
            }
            else if (events[i].filter & EVFILT_READ)
            { //需要读取并处理用户的socket
                clnt_data.clear();
                clnt_socket = event_fd;
                // 由于是边缘触发EV_CLEAR，所以需要一次读完缓冲
                int str_len = 0;
                while (true)
                {
                    // 0:EOF -1:error
                    str_len = read(clnt_socket, buffer, BUFSZ - 1);
                    // empty buffer
                    if (str_len == -1 && errno == EAGAIN)
                        break;
                    buffer[str_len] = '\0';
                    clnt_data += std::string(buffer);
                }
                handler();
                responder();
                close(clnt_socket);
            }
        }
        std::cout << "=====DONE===== " << std::endl;
    }
}