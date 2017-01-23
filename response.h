/* 
 *响应类
 */
#include <iostream>
#include <sstream>
#include "boost/asio.hpp"

namespace webserver{
    class Response:public std::ostream{
    private:
        boost::asio::streambuf buf;
        std::stringstream header;
    public:
        Response():std::ostream(&buf){}
        /* 添加http头信息 */
        void add_header(int status_code,size_t content_length){
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
        void not_found(boost::asio::ip::tcp::socket &client){
            add_header(404,79);
            header<<"<html><head></head><body><h1 align = 'center'>404 Not found!</h1></body></html>";
            boost::asio::write(client,boost::asio::buffer(header.str()));       
        }
        /* 发送信息 */
        void send(boost::asio::ip::tcp::socket &client){
            this->flush();
            size_t content_length = buf.size();
            add_header(200,content_length);
            boost::asio::write(client,boost::asio::buffer(header.str())); 
            boost::asio::write(client,buf); 
        }
    };
}