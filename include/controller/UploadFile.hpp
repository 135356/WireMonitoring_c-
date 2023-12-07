#ifndef BB_UPLOADFILE_H
#define BB_UPLOADFILE_H
#include <map>
#include <string>
#include "mode/bbBasics_uploadFile.hpp"

struct UploadFile{
    UploadFile(std::map<std::string, void (*)(std::map<std::string,std::string> &,std::string &,size_t &)> &route_upload){
        //文件路由请求(客户端上传文件)
        route_upload["/png_file"] = [](std::map<std::string,std::string> &r_data,std::string &s_data,size_t &s_size) {
            s_data.resize(1024);
            int state = bbBasics_uploadFile::obj().insert({{"accounts",r_data["accounts"]},{"file_size",r_data["file_size"]},{"file_type",r_data["file_type"]},{"file_name",r_data["file_name"]},{"file_saved_name",r_data["file_saved_name"]}});
            s_size = sprintf(&s_data[0], R"({"state":%d,"msg":"%s","file_path":"%s"})", 0, "上传成功",r_data["file_saved_name"].c_str());
            s_data.resize(s_size);
        };
    }
};

#endif