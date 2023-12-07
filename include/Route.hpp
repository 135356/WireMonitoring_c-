// http协议的路由地址
// Created by 邦邦 on 2022/6/17.
#ifndef WEBSERVER_ROUTE_H
#define WEBSERVER_ROUTE_H
#include <map>
#include <string>
#include "controller/User.hpp"
#include "controller/UploadFile.hpp"
#include "controller/ConstructionManagement.hpp"

class Route{
    Route(){
        UploadFile upload_file(route_upload);
        
        User user(route);
        ConstructionManagement construction_management(route);
    };
    ~Route() = default;
public:
    static Route &obj(){
        static Route bb_http_route_path;
        return bb_http_route_path;
    }
    //文件路由请求(客户端上传文件)
    std::map<std::string, void (*)(std::map<std::string,std::string> &,std::string &,size_t &)> route_upload{};
    
    /*
     * r_data 接收到的数据
     * s_data 要发送给客户端的数据
     * r_size 要发送的数据大小
     * */
    std::map<std::string, void (*)(unsigned &,std::map<std::string,std::string> &,std::string &,size_t &)> route = {
        //测试
        {"/test",      [](unsigned &client_ip,std::map<std::string,std::string> &r_data,std::string &s_data,size_t &s_size) {
            s_data.resize(1024);
            s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", 0, "成功");
            s_data.resize(s_size);
        }},
    };
};

#endif