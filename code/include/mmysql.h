#pragma once

#include <iostream>
#include <mysql/mysql.h>
#include <glog/logging.h>

using namespace std;
class Mmysql {
public:
    void Init();
    MYSQL* GetConnection();
    void Close();
    int RunQuery(string cmd);
    MYSQL_ROW FetchRes();


private:
    MYSQL* fetchConnection();

    MYSQL* _sql_conn;
};
