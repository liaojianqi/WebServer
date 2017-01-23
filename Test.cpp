#include <iostream>
#include "server.h"
using namespace webserver;
 
int main(){
    unsigned int port = 8888;
    //1. 初始化
    Server server(port);
    //2. 设置路由
    //可以从req对象获取请求信息，将响应信息写入out对象即可，无需手动清空缓冲
    server.router["index.html"]["GET"] = [](const Request &req, std::ostream &out){
        out<<"<html><head></head><body><h1>This is index GET page!</h1></body></html>";
    };
    server.router["login.html"]["GET"] = [port](const Request &req, std::ostream &out){
        out<<"<html><head></head><body><h1>This is login GET page!</h1>"
            "<form action='http://127.0.0.1:" << port << "/doLogin.html' method='post'>"
            "username:<input type='text' name='username'><br>"
            "password:<input type='password' name='password'><br>"
            "<input type='submit'>"
            "</form></body></html>";
    };
    server.router["doLogin.html"]["POST"] = [](const Request &req, std::ostream &out){
        //获取参数信息
        std::string username = "null", password = "null";
        if(req.has_parameter("username"))
            username = req.get_parameter("username");
        if(req.has_parameter("password"))
            password = req.get_parameter("password");
        //返回信息
        out<<"<html><head></head><body><h1>hello,"<<username<<"</h1>"
            <<"<p>Your password is "<<password<<"</p></body></html>";
    };
    server.default_router["GET"] = [](const Request &req, std::ostream &o){
        o<<"<html><head></head><body><h1>This is default GET page!</h1></body></html>";
    };
    //3. 启动服务器
    server.start();
    return 0;
}