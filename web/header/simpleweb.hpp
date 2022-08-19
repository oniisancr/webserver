#ifndef __SIMPLEWEB_H__
#define __SIMPLEWEB_H__
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip> //put_time
namespace HDE
{
    using std::string;
    // 响应报文头
    struct Header
    {
        string protocol = "HTTP/1.1 200 ok\r\n";
        string content_type = "text/html; charset=utf-8\r\n";
        string content_len = "Content-length:0\r\n"; // 消息体长度
        char date[40];
    };

    class SimpleWeb
    {
    private:
        Header *header;
        std::string content;

    public:
        SimpleWeb(std::string url);
        virtual ~SimpleWeb();
        // 获取文件
        std::string get_url_content(const std::string &url);
        // 有的内容 需要解析 如PHP等
        virtual void set_content(std::string &&);
        operator string();
    };
}
#endif // __SIMPLEWEB_H__