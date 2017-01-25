/*
 *http服务器类
 */
#ifndef WEBSERVER_HEAD_SERVER
#define WEBSERVER_HEAD_SERVER

#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include "request.h"
#include "response.h"
#include "boost/asio.hpp"

namespace webserver{
    typedef boost::asio::io_service Ios;
    typedef boost::asio::ip::tcp::endpoint Endpoint;
    typedef boost::asio::ip::tcp::acceptor Acceptor;
    typedef boost::asio::ip::tcp::socket Socket;
    class Server{
        private:
            //监听端口
            const unsigned short port;           
        public:
            /* 构造方法 */
            Server(unsigned short _port):port(_port){}
            /* 启动服务器 */
            void start();
            /* 连接的方法 */
            void do_accept(Acceptor &server,Ios &ios);
            /* 处理请求 */
            void process(Socket &client);
            /* 做出响应 */
            void do_response(const Request &req, Socket &client);
            //用户自定义路由
            std::unordered_map<std::string,std::unordered_map<std::string,
                std::function<void(const Request &req, std::ostream &o)> > > router;
            //用户自定义默认路由
            std::unordered_map<std::string,
                std::function<void(const Request &req, std::ostream &o)> > default_router;
    };
}
#endif