// 用户注册/登陆的mysql数据库mode
// 2023 LongBang
#pragma once
#include "mysqlorm/sql/mode.h"

class bbBasics_user:public mode{
    bbBasics_user(){
        //字段名称，如：名称、年龄、性别，按顺序依次申明
        key_ = {
            "grade",
            "accounts",
            "password"
        };
        //这里是初始化，初始化成功之后将根据mode的名称如(dbA1_test)，自动生成一个db_a1数据库 与 test数据表
        if(initializationF_() != 0){
            bb::secure::Log::obj().error("mode创建的时候出现问题");
        }
        updateF_();
    }
public:
    static auto &obj(){
        static bbBasics_user obj;
        obj.initObj_();
        return obj;
    }
protected:
    //创建表
    int createTableF_(){
        return createTable(table_name_,[this](auto *data){
            data->int_(key_[0])->nullable_()->comment_("等级");
            data->string_(key_[1])->unique_()->comment_("帐号 要求唯一");
            data->string_(key_[2])->nullable_()->comment_("密码");
            data->dateAt_();
        });
    }
    //更新
    void updateF_(){
        /* if(delTable(table_name_) != 0){
            bb::secure::Log::obj().error(DB_name_+",数据表删除失败");
        }
        if(delDB(DB_name_) != 0){
            bb::secure::Log::obj().error(DB_name_+",数据库删除失败");
        }
        bb::secure::Log::obj().info("更新完成请退出程序，并注释掉更新代码"); */
    }
};