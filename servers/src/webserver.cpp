#include "webserver.hpp"

HDE::WebServer::WebServer() : SimpleServer(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10)
{
    timeout = -1;
    server_sock = get_socket()->get_sock();
    // 非阻塞 边缘触发
#ifdef IS_MACOS
    poller = new KququePoller(server_sock, true, true);
#elif IS_LINUX
    poller = new Epoller(server_sock, true, true);
#else
    std::cout<<"Does not support this system"<<std::endl;
#endif

}
HDE::WebServer::~WebServer()
{
    delete poller;
}

void HDE::WebServer::accepter()
{
    int addrlen = sizeof(clnt_addr);
    clnt_socket = accept(server_sock, (sockaddr *)&clnt_addr, (socklen_t *)&addrlen);
    poller->add_socket(clnt_socket);
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
    int event_cnt = 0;
    while (true)
    {
        std::cout << "=====WAITING===== " << std::endl;
        if ((event_cnt = poller->watch(timeout)) == -1)
        {
            std::cout << "epoll_wait error...try again" << std::endl;
            continue;
        }
        // time out
        if (event_cnt == 0)
            continue;

        for (int i = 0; i < event_cnt; i++)
        {
            int temp_socket = poller->eventsVec[i];
            if (temp_socket == server_sock) //有新的连接请求
            {
                accepter();
            }
            else
            { //需要读取并处理用户的socket
                clnt_data.clear();
                clnt_socket = temp_socket;
                // cuz it's edge-triggered, need to read all buffer clearly
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
                // delete client socket
                poller->free_socket(clnt_socket);
                close(clnt_socket);
            }
        }
        std::cout << "=====DONE===== " << std::endl;
    }
}