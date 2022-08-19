#include "webserver.hpp"

HDE::WebServer::WebServer(int port) : SimpleServer(AF_INET, SOCK_STREAM, 0, port, INADDR_ANY, 10)
{
    thread_count = std::thread::hardware_concurrency();

    timeout = -1;
    server_sock = get_socket()->get_sock();
    // 非阻塞 边缘触发
#ifdef IS_MACOS
    poller = new KququePoller(server_sock, true, true);
#elif IS_LINUX
    poller = new Epoller(server_sock, true, true);
#else
    std::cout << "Does not support this system" << std::endl;
#endif
    pool = new ThreadPool(thread_count);
}
HDE::WebServer::~WebServer()
{
    delete poller;
    delete pool;
}

void HDE::WebServer::accepter()
{
    sockaddr_in clnt_addr; //用于保存客户端的信息
    int addrlen = sizeof(clnt_addr);
    int clnt_socket = accept(server_sock, (sockaddr *)&clnt_addr, (socklen_t *)&addrlen);
    poller->add_socket(clnt_socket);
}
void HDE::WebServer::launch()
{
    pool->start(); //启用线程池，处理客户业务
    std::cout << "=====Server Start!===== " << std::endl;
    int event_cnt = 0;
    while (true)
    {
        // std::cout << "=====WAITING===== " << std::endl;
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
            { //交给线程处理用户socket业务
                pool->push_task(new EventHandler(poller, temp_socket));
            }
        }
        // std::cout << "=====DONE===== " << std::endl;
    }
}