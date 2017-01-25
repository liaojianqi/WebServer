#include <iostream>
#include <sstream>
#include "response.h"

namespace webserver{
    /* 添加http头信息 */
    void Response::add_header(std::string::size_type content_length){
        header<<"HTTP/1.1 "<<status_code;
        switch(status_code){
            case 200:header<<" OK\r\n";break;
            case 404:header<<" NOT FOUND\r\n";break;
            default:break;
        }
        header<<"Server:loin/0.1 deepin15.3\r\n";
        header<<"Content-Length: "<<content_length<<"\r\n";
        header<<"\r\n";
    }
    /* 404 not found */
    void Response::not_found(){
        content<<"<html><head></head><body><h1 align = 'center'>"
               <<"404 Not found!</h1></body></html>";
        status_code = 404;
    }
    /* 获取响应信息 */
    std::string Response::get_content(){
        std::string _content = content.str();
        add_header(_content.size());
        header<<_content;
        return header.str();
    }
}


