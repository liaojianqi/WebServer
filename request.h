/*
 *请求类
 */
#ifndef WEBSERVER_HEAD_REQUEST
#define WEBSERVER_HEAD_REQUEST

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

namespace webserver{
    class Request{
    public:
        std::string method, path, version;//请求方法，请求路径，http版本     
        /* 解析请求 */
        void parse_from_istream(std::istream &);
        /* 解析post参数 */
        void parse_parameters(std::istream &);
        /* 头信息是否含有key */
        bool has_attribute(const std::string &) const;
        /* 返回指定头信息 */
        std::string get_attribute(const std::string &) const;
        /* 参数是否含有key */
        bool has_parameter(const std::string &) const;
        /* 返回指定参数 */
        std::string get_parameter(const std::string &) const;     
    private:
        std::unordered_map<std::string,std::string> header;//请求头信息
        std::unordered_map<std::string,std::string> parameter;//请求参数
        /* 工具函数，切割字符串 */
        void split(const std::string &, char , std::vector<std::string> &);
    };
}
#endif
