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
        /* 获取请求方法 */
        std::string get_method() const{
            return method;
        }
        /* 获取请求路径 */
        std::string get_path() const{
            return path;
        }
        /* 获取请求版本号 */
        std::string get_version() const{
            return version;
        }
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
        //请求方法，请求路径，http版本     
        std::string method, path, version;
        //请求头信息
        std::unordered_map<std::string,std::string> header;
        //请求参数
        std::unordered_map<std::string,std::string> parameter;
        /* 工具函数，切割字符串 */
        void split(const std::string &, char , std::vector<std::string> &);
    };
    /* 解析请求 */
    void Request::parse_from_istream(std::istream &in){
        //解析首行
        std::string line;
        std::getline(in,line);
        line.pop_back();
        std::vector<std::string> v;
        this->split(line,' ',v);
        this->method = v[0];
        this->path = v[1].substr(1);
        //是否含有尾部参数
        int pos;
        if((pos = v[2].find('?')) != std::string::npos){
            this->version = v[2].substr(5,pos - 5);
            this->split(v[2].substr(pos + 1),'&',v);
            for(int i=0;i<v.size();i++){
                int location_equal = v[i].find('=');
                parameter[v[i].substr(0,location_equal)] = v[i].substr(location_equal + 1);
            }
        }else{
            this->version = v[2].substr(5);
        }
        //解析头信息
        std::getline(in,line);
        line.pop_back();
        while(line.size() > 0){
            this->split(line,':',v);
            header[v[0]] = v[1].substr(1);
            std::getline(in,line);
            line.pop_back();
        }
    }
    /* 解析post参数 */
    void Request::parse_parameters(std::istream &in){
        std::string line;
        std::vector<std::string> v;
        std::getline(in,line);
        this->split(line,'&',v);
        for(int i=0;i<v.size();i++){
            int location_equal = v[i].find('=');
            parameter[v[i].substr(0,location_equal)] = v[i].substr(location_equal + 1);
        }
    }
    /* 头信息是否含有key */
    bool Request::has_attribute(const std::string &key) const{
        return header.find(key) != header.end();
    }
    /* 返回指定头信息 */
    std::string Request::get_attribute(const std::string &key) const{
        return header.at(key);
    }
    /* 参数是否含有key */
    bool Request::has_parameter(const std::string &key) const{
        return parameter.find(key) != parameter.end();
    }
    /* 返回指定参数 */
    std::string Request::get_parameter(const std::string &key) const{
        return parameter.at(key);
    }
    /* 切割字符串 */
    void Request::split(const std::string &line, char c, std::vector<std::string> &v){
        v.clear();
        std::size_t s = 0, e ;
        while((e = line.find(c, s)) != std::string::npos){
            std::string temp = line.substr(s, e - s);
            if(temp.size() > 0)
                v.push_back(temp);
            s = e + 1;
        }
        v.push_back(line.substr(s));
    }
}
#endif
