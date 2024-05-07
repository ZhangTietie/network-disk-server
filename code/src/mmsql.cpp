#include <gflags/gflags.h>
#include <pthread.h>

#include "mmysql.h"

DECLARE_string(mysql_ip);
DECLARE_int32(mysql_port);
DECLARE_string(mysql_database);
DECLARE_string(mysql_usr);
DECLARE_string(mysql_passwd);

using namespace std;

static pthread_once_t g_mysql_init_id_once = PTHREAD_ONCE_INIT;
static void mysql_init_once(void) {
    mysql_library_init(0, NULL, NULL);
} 

//mysql 初始化不是线程安全的，但是执行语句是线程安全，所以初始化只要执行一次
void Mmysql::Init() {
    pthread_once(&g_mysql_init_id_once, mysql_init_once);
    _sql_conn = fetchConnection();
}

MYSQL* Mmysql::fetchConnection() {
    MYSQL* sql_conn = mysql_init((MYSQL*)0);
    if (!mysql_real_connect(sql_conn, FLAGS_mysql_ip.c_str(), FLAGS_mysql_usr.c_str(),
                            FLAGS_mysql_passwd.c_str(), NULL, FLAGS_mysql_port, NULL, 0)) {
        LOG(WARNING) <<  "mysql connect failed, err: " << strerror(errno);
        return nullptr;
    }
    if (mysql_select_db(sql_conn, FLAGS_mysql_database.c_str())) {
        LOG(WARNING) <<  "mysql select databse failed, err: " << strerror(errno);
        return nullptr;
    }
    return sql_conn;
}

MYSQL* Mmysql::GetConnection() {
    return _sql_conn;
}

void Mmysql::Close() {
    mysql_close(_sql_conn);
}

int Mmysql::RunQuery(string cmd) {
    return mysql_real_query(_sql_conn, cmd.c_str(), cmd.size());
}

MYSQL_ROW Mmysql::FetchRes() {
    MYSQL_RES* sql_res = mysql_store_result(_sql_conn);
    return mysql_fetch_row(sql_res);
}
