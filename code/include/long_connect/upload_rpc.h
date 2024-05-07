#pragma once
#include <brpc/server.h>

#include "mmysql.h"
#include "file_sys.h"
#include "NetDisk.pb.h"
#include "public.h"

class UploadRpc {
public:
    void Init(Mmysql& sql, pthread_mutex_t& sql_mutex);
    void Do(const example::NetDiskRequest* request, example::NetDiskResponse* response);
    
private:
    int partExist(string file_path, string md5);
    bool allPartArrived(string dir, int shard);
    int addSignMap(string sign, string fileName);
    int linkToUsr(string src, string dst, string sign, string fileName);

    FileSys _file_sys;
    Mmysql _sql;
    pthread_mutex_t _sql_mutex;
};
