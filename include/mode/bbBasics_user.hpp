// 用户注册/登陆的mysql数据库mode
// 2023 LongBang
#pragma once
#include "mysqlorm/sql/mode.h"

class bbBasics_user:public mode{
public:
    static auto &obj(){
        static bbBasics_user obj;
        obj.initSqlF_();
        return obj;
    }
private:
    bbBasics_user(){
        key_ = {
            "grade",
            "accounts",
            "password"
        };
        if(initializationF_() != 0){
            bb::secure::Log::obj().error("mode创建的时候出现问题");
        }
        //updateF_();
    }
protected:
    int createTableF_(){
        return createTable([this](auto *data){
            data->int_(key_[0])->nullable_()->comment_("等级");
            data->string_(key_[1])->unique_()->comment_("帐号 要求唯一");
            data->string_(key_[2])->nullable_()->comment_("密码");
            data->dateAt_();
        });
    }
    void updateF_(){
        if(delTable() != 0){
            bb::secure::Log::obj().error(DB_name_+",数据表删除失败");
        }
        if(delDB() != 0){
            bb::secure::Log::obj().error(DB_name_+",数据库删除失败");
        }
        bb::secure::Log::obj().info("更新完成请退出程序，并注释掉更新代码");
    }
};