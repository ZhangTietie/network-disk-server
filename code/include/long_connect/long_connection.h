#pragma once

#include <iostream>
#include <gflags/gflags.h>
#include <brpc/server.h>
#include <bvar/bvar.h>
#include <glog/logging.h>
#include <bvar/multi_dimension.h>
#include <pthread.h>

#include "NetDisk.pb.h"
#include "public.h"
#include "upload_rpc.h"
//#include "download_rpc.h"
#include "mmysql.h"


namespace example {
class NetDiskServiceImpl : public NetDiskService {
public:
    NetDiskServiceImpl();
    virtual ~NetDiskServiceImpl();
    virtual void Send(google::protobuf::RpcController* cntl_base,
                      const NetDiskRequest* request,
                      NetDiskResponse* response,
                      google::protobuf::Closure* done);
private:
    UploadRpc _upload;
    //DownloadRpc _download;
    Mmysql _sql;
    pthread_mutex_t _sql_mutex;
};
}// namespace example

class LongConnectionServer {
public:
    LongConnectionServer(const char* ip, int port): _ip(ip), _port(port) {}
    void Run();
private:
    const char* _ip;
    int _port;
};
