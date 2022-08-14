#include "webserver.hpp"

HDE::WebServer::WebServer() : SimpleServer(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10)
{
    FD_ZERO(&reads);
    // 初始的最大sock值
    fd_max = get_socket()->get_sock();
    server_sock = fd_max;
    // 监听欢迎套接字
    FD_SET(fd_max, &reads);
}

void HDE::WebServer::accepter()
{
    int addrlen = sizeof(clnt_addr);
    new_socket = accept(server_sock, (sockaddr *)&clnt_addr, (socklen_t *)&addrlen);
    FD_SET(new_socket, &reads);            //增加监视的socket
    fd_max = std::max(fd_max, new_socket); //更新socket监视范围
}
void HDE::WebServer::handler()
{
    std::cout << "client addr: " << inet_ntoa(clnt_addr.sin_addr) << ":" << ntohs(clnt_addr.sin_port);
    // std::cout << buffer<<std::endl;
    string url = "";
    int cnt = 0;
    // 取出需要的页面 在第一个空格到第二个空格之间 仅实现GET请求
    for (int i = 0; i < sizeof(buffer); i++)
    {
        char c = buffer[i];
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
    // char *hello = "hello form server";
    // std::string res = "hello form server!";
    SimpleWeb web(target_url);
    write(new_socket, string(web).c_str(), string(web).size());
    close(new_socket);
}
void HDE::WebServer::launch()
{
    int select_num = 0;
    while (true)
    {
        copy_reads = reads;
        timeout.tv_sec = 500;
        timeout.tv_usec = 0;
        std::cout << "=====WAITING===== " << std::endl;
        if ((select_num = select(fd_max + 1, &copy_reads, nullptr, nullptr, &timeout)) == -1)
        {
            std::cout << "select error...try again" << std::endl;
            continue;
        }
        // time out
        if (select_num == 0)
            continue;

        for (int i = 0; i < fd_max + 1; i++)
        {
            if (FD_ISSET(i, &copy_reads))
            {
                if (i == server_sock) //有新的连接请求
                {
                    accepter();
                }
                else
                { //需要读取并处理用户的socket
                    FD_CLR(i, &reads);
                    read(i, buffer, sizeof(buffer));
                    handler();
                    responder();
                    close(i);
                }
            }
        }
        std::cout << "=====DONE===== " << std::endl;
    }
}