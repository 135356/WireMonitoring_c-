#ifndef BB_USER_H
#define BB_USER_H
#include <map>
#include <string>
#include <regex>
#include "bb/Time.h"
#include "bb/secure/Token.h"
#include "FloodIP.hpp"
#include "mode/bbBasics_user.hpp"

struct User{
    User(std::map<std::string, void (*)(unsigned &,std::map<std::string,std::string> &,std::string &,size_t &)> &route){
        //注册
        route["/logon"] = [](unsigned &client_ip,std::map<std::string,std::string> &r_data,std::string &s_data,size_t &s_size) {
            s_data.resize(1024);
            if(!FloodIP::obj().b10.pushIs(client_ip)){
                s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -100, "次数超过限制，稍后再试");
            }else{
                int state = logOn(r_data["accounts"].c_str(), r_data["password"].c_str());
                if (state == 0) {
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", 0, "注册成功");
                } else if(state == -4){
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", state, "帐号已存在");
                }else{
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", state, "注册失败");
                }
            }
            s_data.resize(s_size);
        };
        //登陆
        route["/login"] = [](unsigned &client_ip,std::map<std::string,std::string> &r_data,std::string &s_data,size_t &s_size) {
            s_data.resize(1024);
            if(!FloodIP::obj().a10.pushIs(client_ip)){
                s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -100, "次数超过限制，稍后再试");
            }else{
                std::string token{};
                int state = logIn(r_data["accounts"].c_str(), r_data["password"].c_str(), token);
                if (state == 0) {
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s","token":"%s"})", 0, "登陆成功", token.c_str());
                } else {
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", state, "登陆失败");
                }
            }
            s_data.resize(s_size);
        };
        //token登陆
        route["/login_token"] = [](unsigned &client_ip,std::map<std::string,std::string> &r_data,std::string &s_data,size_t &s_size) {
            s_data.resize(1024);
            if(!FloodIP::obj().b100.pushIs(client_ip)){
                s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -100, "次数超过限制，稍后再试");
            }else{
                if(accessTokenVerification(client_ip,r_data["token"])){
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", 0, "登陆成功");
                }else{
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -1, "登陆失败");
                }
            }
            s_data.resize(s_size);
        };
        //登出
        route["/logout"] = [](unsigned &client_ip,std::map<std::string,std::string> &r_data,std::string &s_data,size_t &s_size) {
            s_data.resize(1024);
            if (bb::secure::Token::obj().rm(r_data["token"])) {
                s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", 0, "登出成功");
            } else {
                s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -1, "登出失败");
            }
            s_data.resize(s_size);
        };
        //修改密码
        route["/change_password"] = [](unsigned &client_ip,std::map<std::string,std::string> &r_data,std::string &s_data,size_t &s_size) {
            s_data.resize(1024);
            if(!FloodIP::obj().a10.pushIs(client_ip)){
                s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -100, "次数超过限制，稍后再试");
            }else{
                if(accessTokenVerification(client_ip,r_data["token"])){
                    int state = changePassword(r_data["accounts"].c_str(), r_data["password"].c_str(),r_data["new_password"].c_str(),r_data["token"]);
                    if (state == 0) {
                        s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", 0, "修改成功");
                    } else {
                        s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", state, "修改失败");
                    }
                }else{
                    s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s"})", -2, "token错误");
                }
            }
            s_data.resize(s_size);
        };
    }
    //access_token验证
    static bool accessTokenVerification(unsigned &client_ip,std::string &access_token){
        if(!FloodIP::obj().a10.is(client_ip)){
            return false;
        }
        if (bb::secure::Token::obj().is(access_token)) {
            return true;
        } else {
            FloodIP::obj().a10.push(client_ip);
            return false;
        }
    }
private:
    //注册
    static int logOn(const char *accounts, const char *password){
        //帐号密码正则验证
        if(!accounts || !password){
            return -1;
        }
        //std::regex regex_a(R"(^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$)");
        std::regex regex_a(R"(\w{5,16})");
        if(!std::regex_search(accounts,regex_a)){
            return -2;
        }
        if(strlen(password) != 32){
            return -3;
        }
        //mysql数据库验证
        std::vector<std::map<std::string, std::string>> data = bbBasics_user::obj().where("accounts",accounts)->get();
        if(!data.empty()){
            return -4;
        }
        int is = bbBasics_user::obj().insert({{"accounts",accounts},{"password",password}});
        if(is < 0){
            return -41;
        }
        return 0;
    }
    //登陆
    static int logIn(const char *accounts, const char *password, std::string &token){
        //帐号密码正则验证
        if(!accounts || !password){
            return -1;
        }
        //std::regex regex_a(R"(^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$)");
        std::regex regex_a(R"(\w{5,16})");
        if(!std::regex_search(accounts,regex_a)){
            return -2;
        }
        if(strlen(password) != 32){
            return -3;
        }
        //mysql数据库验证
        std::vector<std::map<std::string, std::string>> data = bbBasics_user::obj().where("accounts",accounts)->where("password",password)->get();
        if(data.empty()){
            return -4;
        }
        //成功返回token
        bb::secure::Token::obj().push(accounts,token);
        return 0;
    }
    //修改密码
    static int changePassword(const char *accounts, const char *password, const char *new_password,std::string &token){
        //帐号密码正则验证
        //std::regex regex_a(R"(^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$)");
        std::regex regex_a(R"(\w{5,16})");
        if(!std::regex_search(accounts,regex_a)){
            return -2;
        }
        if(strlen(password) != 32 || strlen(new_password) != 32){
            return -3;
        }
        if (!bb::secure::Token::obj().is(token)) {
            return -40;
        }
        bb::secure::Token::obj().rm(token);
        //mysql数据库验证
        std::string data{};
        bbBasics_user::obj().where("accounts",accounts)->where("password",password)->get(data);
        if(data.empty()){
            return -4;
        }
        int is = bbBasics_user::obj().where("accounts",accounts)->where("password",password)->update({{"password",new_password}});
        if(is < 0){
            return -41;
        }
        return 0;
    }
};

#endif