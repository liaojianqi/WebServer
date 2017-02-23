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
        public:
            //端口类型
            typedef unsigned short size_t;
            /* 构造方法 */
            Server(size_t _port):port(_port){}
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
            private:
            //监听端口
            const size_t port;          
    };
    /* 启动服务器 */
    void Server::start(){
        Ios ios;
        Endpoint ep(boost::asio::ip::address_v4::any() ,port);
        Acceptor server(ios, ep.protocol());
        server.bind(ep);
        server.listen();
        
        do_accept(server,ios);
        ios.run();
    }
    /* 连接的方法 */
    void Server::do_accept(Acceptor &server,Ios &ios){
        Socket *client = new Socket(ios);
        std::cout<<"服务器等待建立连接。。。"<<std::endl;
        std::cout<<"监听"<<port<<"端口。。。"<<std::endl;
        server.async_accept(*client,
            [&server,&ios,client,this](const boost::system::error_code &ec){
                if(ec){
                    std::cout<<"连接失败:"<<ec.message()<<std::endl;
                }else{
                    std::cout<<"一个客户端建立连接"<<std::endl;
                    do_accept(server,ios);
                    process(*client);
                }
            }
        );
    }
    /* 处理请求 */
    void Server::process(Socket &client){
        std::cout<<"进入process方法"<<std::endl;
        boost::asio::streambuf *read_buf = new boost::asio::streambuf;
        std::cout<<"read_until之前"<<std::endl;
        boost::asio::async_read_until(client,*read_buf,"\r\n\r\n",
            [&client,read_buf,this](const boost::system::error_code &ec,std::size_t cnt){
                if(ec){
                    std::cout<<"读入错误："<<ec.message()<<std::endl;
                }else{
                    std::size_t total = read_buf->size();
                    std::istream in(read_buf);
                    Request req;
                    std::cout<<"开始解析请求信息"<<std::endl;
                    req.parse_from_istream(in);
                    std::cout<<"解析请求信息完成"<<std::endl;
                    if(req.has_attribute("Content-Length")){
                        size_t rest = stoull(req.get_attribute("Content-Length"));
                        boost::asio::read(client,*read_buf,
                            boost::asio::transfer_exactly(rest - (total - cnt)));
                        std::istream i(read_buf);
                        req.parse_parameters(i);
                    }
                    do_response(req,client);
                    delete read_buf;
                    process(client);
                }
            }
        );
        std::cout<<"read_until之后"<<std::endl;
    }
    /* 做出响应 */
    void Server::do_response(const Request &req, Socket &client){
        std::cout<<"进入do_response方法"<<std::endl;
        Response rsp;
        if(req.get_path().size() != 0){
            if(router[req.get_path()].find(req.get_method()) != router[req.get_path()].end()){
                router[req.get_path()][req.get_method()](req,rsp.get_ostream());
            }else{
                rsp.not_found();
            }
        }else{
            if(default_router.find(req.get_method()) != default_router.end()){
                default_router[req.get_method()](req,rsp.get_ostream());
            }else{
                rsp.not_found();
            }
        }     
        std::string content = rsp.get_content();
        boost::asio::async_write(client,boost::asio::buffer(content),
            [](const boost::system::error_code &ec, std::size_t len){
                if(ec){
                    std::cout<<"写信息错误！"<<std::endl;            
                }
            }
        );
        std::cout<<"离开do_response方法"<<std::endl;
    } 
}
#endif