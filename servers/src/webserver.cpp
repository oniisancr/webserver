#include "webserver.hpp"

HDE::WebServer::WebServer() : SimpleServer(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10)
{
    event_cnt = 0;
    // -1 means waitinng until client socke be accepted
    timeout = -1; 
    // initate epfd
    epfd = epoll_create(EPOLLSZ);
    server_sock = get_socket()->get_sock();
    //set nonblock socket
    int flag = fcntl(server_sock,F_GETFL,0);
    fcntl(server_sock,F_SETFL,flag|O_NONBLOCK);

    epoll_evens = new epoll_event[EPOLLSZ];
    //watch server fd  (edge-triggered)
    event.events = EPOLLIN|EPOLLET;
    event.data.fd = server_sock;
    epoll_ctl(epfd,EPOLL_CTL_ADD,server_sock,&event);
}
HDE::WebServer::~WebServer()
{
    delete [] epoll_evens;
}

void HDE::WebServer::accepter()
{
    int addrlen = sizeof(clnt_addr);
    clnt_socket = accept(server_sock, (sockaddr *)&clnt_addr, (socklen_t *)&addrlen);
    //set nonblock socket
    int flag = fcntl(clnt_socket,F_GETFL,0);
    fcntl(clnt_socket,F_SETFL,flag|O_NONBLOCK);
    //edge-triggered
    event.events = EPOLLIN|EPOLLET;
    event.data.fd = clnt_socket;
    epoll_ctl(epfd,EPOLL_CTL_ADD,clnt_socket,&event);
}
void HDE::WebServer::handler()
{
    std::cout << "client addr: " << inet_ntoa(clnt_addr.sin_addr) << ":" << ntohs(clnt_addr.sin_port);
    string url = "";
    int cnt = 0;
    // 取出需要的页面 在第一个空格到第二个空格之间 仅实现GET请求
    for (int i = 0; i<clnt_data.size(); i++)
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
    std::cout <<" --> "<< url << std::endl;
    target_url = url;
}
void HDE::WebServer::responder()
{
    SimpleWeb web(target_url);
    write(clnt_socket, string(web).c_str(), string(web).size());
}
void HDE::WebServer::launch()
{
    int select_num = 0;
    while (true)
    {
        std::cout << "=====WAITING===== " << std::endl;
        if ((event_cnt = epoll_wait(epfd, epoll_evens, EPOLLSZ,timeout)) == -1)
        {
            std::cout << "epoll_wait error...try again" << std::endl;
            continue;
        }
        // time out
        if (event_cnt == 0)
            continue;

        for (int i = 0; i < event_cnt; i++)
        {
            
            if (epoll_evens[i].data.fd == server_sock) //有新的连接请求
            {
                accepter();
            }
            else
            { //需要读取并处理用户的socket
                clnt_data.clear();
                clnt_socket = epoll_evens[i].data.fd;
                // cuz it's edge-triggered, need to read all buffer clearly
                int str_len=0;
                while(true)
                {
                    // 0:EOF -1:error
                    str_len=read(clnt_socket, buffer, BUFSZ-1);
                    //empty buffer  
                    if(str_len == -1 && errno == EAGAIN)
                        break;
                    buffer[str_len]='\0';
                    clnt_data += std::string(buffer);
                }
                handler();
                responder();
                // delete client socket
                epoll_ctl(epfd, EPOLL_CTL_DEL, clnt_socket, NULL);
                close(clnt_socket);
            }
        }
        std::cout << "=====DONE===== " << std::endl;
    }
}