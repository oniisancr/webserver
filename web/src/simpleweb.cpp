#include "simpleweb.hpp"

HDE::SimpleWeb::SimpleWeb(std::string url)
{
    header = new Header;
    set_content(get_url_content(url));
}

std::string HDE::SimpleWeb::get_url_content(const std::string& url)
{
    std::ifstream urlfile("webcontent/"+url);
    string ret="";
    // url文件是否存在
    if(urlfile.is_open())
    {
        string line;
        while(getline(urlfile,line))
        {
            ret+=line+"\n";
        }
    }
    else{ // 否则返回 404页面
        if(url != "404.html")
            ret = get_url_content("404.html");
        else ret = "404 NOT FOUND!";
        header->statusCode = 404;
        header->statusStr = "not found";
    }
    time_t now = time(nullptr); 
    std::strftime(header->date, sizeof(header->date), "%c %Z", std::gmtime(&now));
    // std::cout<<std::put_time(std::gmtime(&now),"%c %Z");
    return ret;
}

void HDE::SimpleWeb::set_content(std::string&& con)
{
    content = con;
}
// 转换成字符串
HDE::SimpleWeb::operator string()
{
    string headerStr="";
    headerStr+=header->protocol+" ";
    headerStr+=std::to_string(header->statusCode)+" ";
    headerStr+=header->statusStr+"\n";
    headerStr+="Content-Type: "+header->content_type+"\n";
    headerStr+="Date: "+string(header->date)+"\n";
    headerStr+="Content-Length:"+std::to_string(content.size())+"\n";
    
    headerStr+="\n";
    return headerStr+content;
}