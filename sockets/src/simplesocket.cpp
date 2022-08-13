#include "simplesocket.hpp"

/***
 * domain 地址协议类型：AF_INET TCP/IP协议家族 ipv4
 * type 套接字类型：SOCK_STREAM TCP, SOCK_DGRAM UDP
 * protocol
 * port 监听端口
 * interface 本机IP地址 INADDR_ANY 0.0.0.0 本机所有ip地址
 */
HDE::SimpleSocket::SimpleSocket(int domain, int type, int protocol, int port,
                                u_long interface)
{
    // 定义结构体
    address.sin_family = domain;
    address.sin_port = htons(port); // htons() 16位的主机字节序转换到网络字节序 大端模式
    address.sin_addr.s_addr = htonl(interface); //host to net long 32位的主机字节序转换到网络字节序
    // 创建服务器套接字
    sock = socket(domain, type, protocol);
    test_connection(sock);
    
}
void HDE::SimpleSocket::test_connection(int item_to_test)
{
    if(item_to_test<0)
    {
        perror("Failed to connect...");
        exit(EXIT_FAILURE);
    }
}
sockaddr_in HDE::SimpleSocket::get_address()
{
    return address;
}
int HDE::SimpleSocket::get_sock()
{
    return sock;
}
int HDE::SimpleSocket::get_connection()
{
    return connection;
}
void HDE::SimpleSocket::set_connection(int conn)
{
    connection = conn;
}