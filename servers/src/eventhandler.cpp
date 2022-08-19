#include "eventhandler.hpp"

HDE::EventHandler::EventHandler(SimplePoller *ptr, int skt) : poller(ptr), clnt_socket(skt)
{
}
HDE::EventHandler::~EventHandler()
{
    poller->free_socket(clnt_socket);
    close(clnt_socket);
}
void HDE::EventHandler::handler()
{
    // 只处理GET请求
    if (clnt_data.find("HTTP/") == std::string::npos||clnt_data.find("GET") == std::string::npos)
    {
        target_url = "";
        return;
    }
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
    target_url = url;
    // 查看连接信息
    sockaddr_in clnt_addr; //用于保存客户端的信息
    int addrlen = sizeof(clnt_addr);
    getpeername(clnt_socket, (sockaddr *)&clnt_addr, (socklen_t *)&addrlen);
    std::cout << "client addr: " << inet_ntoa(clnt_addr.sin_addr) << ":"
              << ntohs(clnt_addr.sin_port) << " --> " << url << std::endl;
}
void HDE::EventHandler::responder()
{
    if(target_url.empty()) return;
    SimpleWeb web(target_url);
    //客户端已经 断开
    if (-1 == write(clnt_socket, string(web).c_str(), string(web).size()) && errno == EPIPE)
    {
        // std::cout << "client disconnected" << std::endl;
    }
    // 若写缓冲区满  -1  EWOULDBLOCK/EAGAIN
    /*
        暂时放弃继续发送
    */
}
void HDE::EventHandler::run()
{
    // cuz it's edge-triggered, need to read all buffer clearly
    int str_len = 0;
    while (true)
    {
        // 0:EOF -1:error
        str_len = read(clnt_socket, buffer, BUFSZ - 1);
        if (str_len == 0) //客户端断开、或者读完数据
            break;
        // empty buffer
        if (str_len == -1 && errno == EAGAIN)
            break;
        buffer[str_len] = '\0';
        clnt_data += std::string(buffer);
    }
    if (!clnt_data.empty()) //无数据就直接关闭
    {
        handler();
        responder();
    }
}