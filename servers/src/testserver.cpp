#include "testserver.hpp"

HDE::TestServer::TestServer() : SimpleServer(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY, 10)
{
}

void HDE::TestServer::accepter()
{
    sockaddr_in address = get_socket()->get_address();

    int addrlen = sizeof(address);
    new_socket = accept(get_socket()->get_sock(), (sockaddr *)&address, (socklen_t *)&addrlen);
    read(new_socket, buffer, sizeof(buffer));
}
void HDE::TestServer::handler()
{
    std::cout << buffer << std::endl;
}
void HDE::TestServer::responder()
{
    // char *hello = "hello form server";
    std::string res = "hello form server!";
    write(new_socket, res.c_str(), res.size());
    close(new_socket);
}
void HDE::TestServer::launch()
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