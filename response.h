/* 
 *响应类
 */
#ifndef WEBSERVER_HEAD_RESPONSE
#define WEBSERVER_HEAD_RESPONSE

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
        void add_header(int status_code,size_t content_length);
        /* 404 not found */
        void not_found(boost::asio::ip::tcp::socket &client);
        /* 发送信息 */
        void send(boost::asio::ip::tcp::socket &client);
    };
}
#endif