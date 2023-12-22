get_filename_component(project_name_ ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(TOLOWER ${project_name_} project_name_)
cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 20)
project(${project_name_} VERSION 6.0.0)
ADD_DEFINITIONS(-D_PROJECT_VERSION=\"${PROJECT_VERSION}\")

#Debug｜Release模式: -g表示带调试信息，-Wall表示输出所有的警告信息
set(CMAKE_BUILD_TYPE Release) #cmake根据编译器加-g参数(调试)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(install_dir /usr/local/bb)
else()
    set(install_dir ${CMAKE_SOURCE_DIR}/build)
endif()

set(CMAKE_PREFIX_PATH ${install_dir}/cmake/modules)
find_package(bb 6.0 REQUIRED NO_MODULE)
find_package(mysql REQUIRED NO_MODULE)
find_package(mysqlorm REQUIRED NO_MODULE)
include_directories(${bb_INCLUDES} ${mysql_INCLUDES} ${mysqlorm_INCLUDES} "include")
add_executable(${PROJECT_NAME}_run main.cpp)
target_link_libraries(${PROJECT_NAME}_run -lpthread bb mysqlorm ${mysql_LIBS})