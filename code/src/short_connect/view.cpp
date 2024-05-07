
#include "view.h"

int View::Init() {
    _sql.Init();
    return 0;
}

View::~View() {
    _sql.Close();
}

/*
MYSQL* View::getSqlConn() {
    return _sql_conn;
}
*/

FileSys& View::getFileSys() {
    return _file_sys;
}
