#pragma once
#include<jsoncpp/json/json.h>

#include "mmysql.h"
#include "file_sys.h"
#include "public.h"

using namespace std;

//视图文件基类
class View {
public:
    ~View();
    int Init();
    int Destroy();
    MYSQL* getSqlConn();
    FileSys& getFileSys();

    virtual void Process(Json::Value val,int client_fd) = 0;
public:
    Mmysql _sql;

private:
    FileSys _file_sys;
};
