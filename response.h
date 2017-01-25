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
}
#endif