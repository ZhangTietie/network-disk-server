#include <iostream>

#include "long_connection.h"

DECLARE_int32(idle_timeout_s);

namespace example {
NetDiskServiceImpl::NetDiskServiceImpl() {
    pthread_mutex_init(&_sql_mutex, NULL);
    _sql.Init();
    _upload.Init(_sql, _sql_mutex);
    //_download.Init(_sql);
}

NetDiskServiceImpl::~NetDiskServiceImpl() {
    _sql.Close();
}

void NetDiskServiceImpl::Send(google::protobuf::RpcController* cntl_base,
                      const NetDiskRequest* request,
                      NetDiskResponse* response,
                      google::protobuf::Closure* done) {
    brpc::ClosureGuard done_guard(done);
    brpc::Controller* cntl = static_cast<brpc::Controller*>(cntl_base);;
    LOG(INFO) << "LongConnectionServer received request"
                << "] from " << cntl->remote_side()
                << " to " << cntl->local_side()
                << ". op_type " << request->op_type();

    response->set_file_name(request->file_name());
    response->set_shard(request->shard());
    response->set_part_index(request->part_index());
    switch (request->op_type()) {
        case static_cast<int>(MSG_TYPE::UPLOAD):
            _upload.Do(request, response);
            break;
        case static_cast<int>(MSG_TYPE::DOWNLOAD):
            //_download.Do(request, response);
            break;
        default:
            response->set_res(false);
            response->set_body("op not support");
            LOG(WARNING) << "not support " << request->op_type();
        }
        if (response->res()) {
            response->set_body("upload success");
        }
}
}// namespace example



void LongConnectionServer::Run() {
    LOG(INFO) << "LongConnectionServer start run";
    brpc::Server server;
    example::NetDiskServiceImpl netdisk_service_impl;

    if (server.AddService(&netdisk_service_impl, brpc::SERVER_DOESNT_OWN_SERVICE) != 0) {
        LOG(FATAL) << "Fail to add service";
    }
    butil::EndPoint point;
    point = butil::EndPoint(butil::IP_ANY, _port);
    
    brpc::ServerOptions options;
    options.idle_timeout_sec = FLAGS_idle_timeout_s;
    if (server.Start(point, &options) != 0) {
        LOG(FATAL) << "Fail to start NetDiskServer long connectrion";
    }

    // Wait until Ctrl-C is pressed, then Stop() and Join() the server.
    server.RunUntilAskedToQuit();
    return;
}
