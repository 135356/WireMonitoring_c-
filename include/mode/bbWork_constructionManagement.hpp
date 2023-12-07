// 用户注册/登陆的mysql数据库mode
// 2023 LongBang
#pragma once
#include "mysqlorm/sql/mode.h"

class bbWork_constructionManagement:public mode{
    bbWork_constructionManagement(){
        //字段名称，如：名称、年龄、性别，按顺序依次申明
        key_ = {
            "name",
            "area",
            "area_name",
            "undertake_department",
            "undertake_person",
            "report_person",
            "report_phone",
            "risk_level",
            "state",
            "MP_date_start",
            "MP_date_end",
            "notes"
        };
        //这里是初始化，初始化成功之后将根据mode的名称如(dbA1_test)，自动生成一个db_a1数据库 与 test数据表
        if(initializationF_() != 0){
            bb::secure::Log::obj().error("mode创建的时候出现问题");
        }
        updateF_();
    }
public:
    static auto &obj(){
        static bbWork_constructionManagement obj;
        obj.initObj_();
        return obj;
    }
protected:
    //创建表
    int createTableF_(){
        return createTable(table_name_,[this](auto *data){
            data->string_(key_[0])->comment_("项目名称");
            data->string_(key_[1])->nullable_()->comment_("所属片区");
            data->string_(key_[2])->nullable_()->comment_("区间名称");
            data->string_(key_[3])->comment_("负责部门");
            data->string_(key_[4])->comment_("负责人");
            data->string_(key_[5])->comment_("上报人");
            data->string_(key_[6])->comment_("上报人联系方式");
            data->int_(key_[7])->comment_("风险级别");
            data->int_(key_[8])->comment_("状态");
            data->string_(key_[9])->comment_("监控时段开始");
            data->string_(key_[10])->comment_("监控时段结束");
            data->string_(key_[11])->nullable_()->comment_("备注");
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