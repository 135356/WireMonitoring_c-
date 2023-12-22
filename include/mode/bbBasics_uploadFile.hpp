// 上传文件的mysql数据库mode
// 2023 LongBang
#pragma once
#include "mysqlorm/sql/mode.h"

class bbBasics_uploadFile:public mode{
public:
    static auto &obj(){
        static bbBasics_uploadFile obj;
        obj.initSqlF_();
        return obj;
    }
private:
    bbBasics_uploadFile(){
        key_ = {
            "accounts",
            "file_size",
            "file_type",
            "file_name",
            "file_saved_name"
        };
        if(initializationF_() != 0){
            bb::secure::Log::obj().error("mode创建的时候出现问题");
        }
        //updateF_();
    }
protected:
    //创建表
    int createTableF_(){
        return createTable([this](auto *data){
            data->string_(key_[0])->nullable_()->comment_("帐号");
            data->string_(key_[1])->nullable_()->comment_("文件大小");
            data->string_(key_[2])->nullable_()->comment_("文件类型");
            data->string_(key_[3])->nullable_()->comment_("文件名称");
            data->string_(key_[4])->nullable_()->comment_("文件保存名称");
            data->dateAt_();
        });
    }
    //更新
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