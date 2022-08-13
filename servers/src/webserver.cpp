#include "webserver.hpp"

HDE::WebServer::WebServer() : SimpleServer(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10)
{
}

void HDE::WebServer::accepter()
{
    // sockaddr_in address = get_socket()->get_address();
    int addrlen = sizeof(clnt_addr);
    new_socket = accept(get_socket()->get_sock(), (sockaddr *)&clnt_addr, (socklen_t *)&addrlen);
    read(new_socket, buffer, sizeof(buffer));
}
void HDE::WebServer::handler()
{
    std::cout << "client addr: " << inet_ntoa(clnt_addr.sin_addr) << ":" << ntohs(clnt_addr.sin_port) << std::endl;
    // std::cout << buffer<<std::endl;
    string url = "";
    int cnt = 0;
    // 取出需要的页面 在第一个空格到第二个空格之间 仅实现GET请求
    for (int i = 0; i < sizeof(buffer); i++)
    {
        char c = buffer[i];
        if (cnt == 1 && c != ' '&&c != '/')
            url += c;
        if (c == ' ')
            cnt++;
        if (cnt > 1)
            break;
    }
    if (url.empty())
        url = "index.html";
    std::cout << url << std::endl;
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
    while (true)
    {
        std::cout << "=====WAITING===== " << std::endl;
        accepter();
        handler();
        responder();
        std::cout << "=====DONE===== " << std::endl;
    }
}