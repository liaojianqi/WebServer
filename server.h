/*
 *http服务器类
 */
#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <boost/asio.hpp>
#include "request.h"

namespace webserver{
    class Server{
        private:
            //监听端口
            const unsigned short port;           
        public:
            /* 构造方法 */
            Server(unsigned short _port):port(_port){}
            /* 启动服务器 */
            void start(){
                boost::asio::io_service ios;
                boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any() ,port);
                boost::asio::ip::tcp::acceptor server(ios, ep.protocol());
                server.bind(ep);
                server.listen();
                while(true){
                    boost::asio::ip::tcp::socket client(ios);
                    server.accept(client);
                    std::cout<<"一个客户端建立连接"<<std::endl;
                    process(client);
                }
            }
            /* 404 not found */
            void not_found(std::ostream &out){
                add_header(out,404);
                out<<"<html><head></head><body><h1 align = 'center'>404 Not found!</h1></body></html>";
            }
            /* 添加http头信息 */
            void add_header(std::ostream &out, int status_code){
                out<<"HTTP/1.1 "<<status_code;
                switch(status_code){
                    case 200:out<<"OK\r\n";break;
                    case 404:out<<"NOT FOUND\r\n";break;
                    default:break;
                }
                out<<"Server:loin/0.1 deepin15.3\r\n";
                out<<"\r\n";
            }
            /* 做出响应 */
            void do_response(const Request &req, boost::asio::ip::tcp::socket &client){
                boost::asio::streambuf write_buf;
                std::ostream out(&write_buf);
                if(req.path.size() != 0){
                    if(router[req.path].find(req.method) != router[req.path].end()){
                        add_header(out,200);
                        router[req.path][req.method](req,out);
                    }else{
                        //404
                        not_found(out);
                    }
                }else{
                    if(default_router.find(req.method) != default_router.end()){
                        //返回默认页面
                        add_header(out,200);
                        default_router[req.method](req,out);
                    }else{
                        //404
                        not_found(out);
                    }
                }
                out.flush();
                //返回信息
                boost::asio::write(client,write_buf);             
            }         
            /* 处理请求 */
            void process(boost::asio::ip::tcp::socket &client){
                /*
                std::cout<<"进入process方法"<<std::endl;
                boost::asio::streambuf read_buf;   
                char *s_c = new char[1024]; 
                //boost::asio::buffer(s_c,1024)
                boost::asio::async_read_until(client,boost::asio::buffer(s_c,1024),"\r\n\r\n",
                    [](const boost::system::error_code &e, std::size_t cnt){
                        if(!e)
                            std::cout<<"读入完成"<<std::endl;
                        else
                            std::cout<<"出现错误"<<std::endl;
                    }
                );
                /*
                    [&client,&read_buf,this](const boost::system::error_code &e, std::size_t cnt){
                        std::cout<<"读入完成"<<std::endl;
                        if(!e){
                            std::size_t total = read_buf.size();
                            std::istream in(&read_buf);
                            Request req;
                            std::cout<<"开始解析请求信息"<<std::endl;
                            req.parse(in);
                            std::cout<<"解析请求信息完成"<<std::endl;
                            if(req.has_attribute("Content-Length")){
                                size_t rest = stoull(req.get_attribute("Content-Length"));
                                boost::asio::read(client,read_buf,
                                    boost::asio::transfer_exactly(rest - (total - cnt)));
                                std::istream i(&read_buf);
                                req.parse_parameters(i);
                            }
                            do_response(req,client);
                        }else{
                            std::cout<<"error occur"<<std::endl;
                        }
                  
                });
                  */    
                  Request req;
                  do_response(req,client);
            }
            //用户自定义路由
            std::unordered_map<std::string,std::unordered_map<std::string,
                std::function<void(const Request &req, std::ostream &o)> > > router;
            //用户自定义默认路由
            std::unordered_map<std::string,
                std::function<void(const Request &req, std::ostream &o)> > default_router;
    };
}