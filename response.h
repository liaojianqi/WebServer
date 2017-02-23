/* 
 *响应类
 */
#ifndef WEBSERVER_HEAD_RESPONSE
#define WEBSERVER_HEAD_RESPONSE

#include <iostream>
#include <sstream>

namespace webserver{
    class Response{
    private:
        //状态码
        int status_code = 200;
        //http响应报文头信息
        std::stringstream header;
        //响应正文内容
        std::ostringstream content;
    public:
        /* 获取输出流 */
        std::ostream& get_ostream(){
            return content;
        }
        /* 添加http头信息 */
        void add_header(std::string::size_type content_length);
        /* 404 not found */
        void not_found();
        /* 获取响应信息 */
        std::string get_content();
    };
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
#endif