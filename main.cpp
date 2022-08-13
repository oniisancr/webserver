#include <iostream>
#include "webserver.hpp"

int main(int, char**) {
    using namespace HDE;
    // std::cout << "binding socket!\n";
    // BindingSocket bs = BindingSocket(AF_INET,SOCK_STREAM,0,8080,INADDR_ANY);
    // std::cout << "listening socket!\n";
    // ListeningSocket ls = ListeningSocket(AF_INET,SOCK_STREAM,0,8080,INADDR_ANY,10);
    // std::cout << "test server!\n";
    // TestServer server;
    //server.launch();
    // std::cout << "success!\n";
    // SimpleWeb web("index.html");
    // std::cout<<string(web)<<std::endl;
    WebServer server;
    server.launch();
    return 0;
}
