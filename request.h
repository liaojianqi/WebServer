/*
 *请求类
 */
#include <string>
#include <unordered_map>

namespace webserver{
    class Request{
    public:
        std::string method, path, version;//请求方法，请求路径，http版本
        std::unordered_map<std::string,std::string> header;//请求头信息
        std::unordered_map<std::string,std::string> parameter;//请求参数
    };
}
