//施工管理
#ifndef WEB_SERVER_CONSTRUCTIONMANAGEMENT_H
#define WEB_SERVER_CONSTRUCTIONMANAGEMENT_H
#include <map>
#include <string>
#include "controller/User.hpp"
#include "mode/bbWork_constructionManagement.hpp"

struct ConstructionManagement{
    ConstructionManagement(std::map<std::string, void (*)(unsigned &,std::map<std::string,std::string> &,std::string &,size_t &)> &route){
        route["/ConstructionManagementC"] = [](unsigned &client_ip,std::map<std::string,std::string> &r_data,std::string &s_data,size_t &s_size) {
            s_data.resize(1024);
            if(!FloodIP::obj().b100.pushIs(client_ip)){
                s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -100, "次数超过限制，稍后再试");
            }else{
                if(User::accessTokenVerification(client_ip,r_data["token"])){
                    int is = bbWork_constructionManagement::obj().insert({
                        {"name",r_data["name"]},
                        {"area",r_data["area"]},
                        {"area_name",r_data["area_name"]},
                        {"undertake_department",r_data["undertake_department"]},
                        {"undertake_person",r_data["undertake_person"]},
                        {"report_person",r_data["report_person"]},
                        {"report_phone",r_data["report_phone"]},
                        {"risk_level",r_data["risk_level"]},
                        {"state",r_data["state"]},
                        {"MP_date_start",r_data["MP_date_start"]},
                        {"MP_date_end",r_data["MP_date_end"]},
                        {"notes",r_data["notes"]},
                    });
                    if(is < 0){
                        s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -1, "失败");
                    }else{
                        s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", 0, "成功");
                    }
                }else{
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -2, "token错误");
                }
            }
            s_data.resize(s_size);
        };
        route["/ConstructionManagementR"] = [](unsigned &client_ip,std::map<std::string,std::string> &r_data,std::string &s_data,size_t &s_size) {
            std::string str_data{};
            if(!FloodIP::obj().b100.pushIs(client_ip)){
                s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -100, "次数超过限制，稍后再试");
            }else{
                if(User::accessTokenVerification(client_ip,r_data["token"])){
                    std::string str_data{};int is = -1;
                    if(r_data["page_type"] == "0"){ //指定页
                        is = bbWork_constructionManagement::obj().limitSpecific(str_data,r_data["page"],r_data["length"]);
                    }else if(r_data["page_type"] == "-1"){ //上翻页
                        is = bbWork_constructionManagement::obj().limitLast(str_data,r_data["id"],r_data["length"]);
                    }else if(r_data["page_type"] == "1"){ //下翻页
                        is = bbWork_constructionManagement::obj().limitNext(str_data,r_data["id"],r_data["length"]);
                    }
                    /* std::string aaa{};
                    for(int i=0;i<90000;i++){
                        aaa+=str_data+",";
                    }
                    aaa.pop_back();
                    str_data = "["+aaa+"]"; */
                    s_data.resize(str_data.size()+1024);
                    if(is < 0){
                        s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -1, "失败");
                    }else{
                        s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s","cont":%s})",0, "成功",str_data.c_str());
                    }
                }else{
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -2, "token错误");
                }
            }
            s_data.resize(s_size);
        };
        route["/ConstructionManagementU"] = [](unsigned &client_ip,std::map<std::string,std::string> &r_data,std::string &s_data,size_t &s_size) {
            s_data.resize(1024);
            if(!FloodIP::obj().b100.pushIs(client_ip)){
                s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -100, "次数超过限制，稍后再试");
            }else{
                if(User::accessTokenVerification(client_ip,r_data["token"])){
                    int is = bbWork_constructionManagement::obj().where("id",r_data["id"])->update({
                        {"name",r_data["name"]},
                        {"area",r_data["area"]},
                        {"area_name",r_data["area_name"]},
                        {"undertake_department",r_data["undertake_department"]},
                        {"undertake_person",r_data["undertake_person"]},
                        {"report_person",r_data["report_person"]},
                        {"report_phone",r_data["report_phone"]},
                        {"risk_level",r_data["risk_level"]},
                        {"state",r_data["state"]},
                        {"MP_date_start",r_data["MP_date_start"]},
                        {"MP_date_end",r_data["MP_date_end"]},
                        {"notes",r_data["notes"]}
                    });
                    if(is < 0){
                        s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -1, "失败");
                    }else{
                        s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", 0, "成功");
                    }
                }else{
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -2, "token错误");
                }
            }
            s_data.resize(s_size);
        };
        route["/ConstructionManagementD"] = [](unsigned &client_ip,std::map<std::string,std::string> &r_data,std::string &s_data,size_t &s_size) {
            s_data.resize(1024);
            if(!FloodIP::obj().b100.pushIs(client_ip)){
                s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -100, "次数超过限制，稍后再试");
            }else{
                if(User::accessTokenVerification(client_ip,r_data["token"])){
                    int is = bbWork_constructionManagement::obj().where("id",r_data["id"])->del();
                    if(is < 0){
                        s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -1, "失败");
                    }else{
                        s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", 0, "成功");
                    }
                }else{
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -2, "token错误");
                }
            }
            s_data.resize(s_size);
        };
    }
};

#endif