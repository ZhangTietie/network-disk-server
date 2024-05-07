#include <iostream>


#include "brpc_client.h"
#include "md5.h"
#include "public.h"

using namespace std;

DECLARE_string(protocol);
DECLARE_string(connection_type);
DECLARE_int32(timeout_ms);
DECLARE_int32(max_retry);
DECLARE_string(load_balancer);

#define MAX_SIZE_PER_SHARD 2

int BrpcClient::Init() {
    /*
    brpc::Channel channel;
    brpc::ChannelOptions options;
    options.protocol = FLAGS_protocol;
    options.connection_type = FLAGS_connection_type;
    options.timeout_ms = FLAGS_timeout_ms;
    options.max_retry = FLAGS_max_retry;
    if (channel.Init(_server.c_str(), FLAGS_load_balancer.c_str(), &options) != 0) {
        LOG(ERROR) << "Fail to initialize channel";
        return -1;
    }
    _stub = new example::NetDiskService_Stub(&channel);
    */
    return 0;
}

int BrpcClient::DoRequest(const int type, string file, string usr) {
    brpc::Channel channel;
    brpc::ChannelOptions options;
    options.protocol = FLAGS_protocol;
    options.connection_type = FLAGS_connection_type;
    options.timeout_ms = FLAGS_timeout_ms/*milliseconds*/;
    options.max_retry = FLAGS_max_retry;
    if (channel.Init(_server.c_str(), FLAGS_load_balancer.c_str(), &options) != 0) {
        LOG(ERROR) << "Fail to initialize channel";
        return -1;
    }
    _stub = new example::NetDiskService_Stub(&channel);

    if (type == static_cast<int>(MSG_TYPE::UPLOAD)) {
        doUploadFile(file, usr);
    } else {
        LOG(WARNING) << "not support in brpc client DoRequest";
    }
    return 0;
}

long getFileSize(FILE* file) {
    long fileSize = -1;
    if (file != NULL) {
        if (fseek(file, 0L, SEEK_END) == 0) {
            fileSize = ftell(file);
        }
        rewind(file);
    }
    return fileSize;
}
int BrpcClient::doUploadFile(string file, string usr) {
    string file_md5;
    if (Md5Sum(file, file_md5) != 0) {
        LOG(WARNING) << "cal file md5 failed";
        return 0;
    }
    //读文件
    //拆分文件，每个文件多大
    //循环遍历每个shard，进行上传
    //所有shard上传完成，上传结束
    FILE*  fp_s = fopen(file.c_str(), "rb");
    if (fp_s == NULL) {
        LOG(WARNING) << "open upload file failed";
        fclose(fp_s);
        return 0;
    }
    int size = getFileSize(fp_s);
    fclose(fp_s);

    FILE* fp = fopen(file.c_str(), "rb");
    if (fp == NULL) {
        LOG(WARNING) << "open upload file failed";
        fclose(fp);
        return 0;
    }
    int shard = size / MAX_SIZE_PER_SHARD + 1;
    char** buffer = (char**) malloc (sizeof(char*) * shard);
    for (int part_index = 0; part_index < shard; ++part_index) {
        example::NetDiskRequest request;
        example::NetDiskResponse response;
        brpc::Controller cntl;
        request.set_op_type(static_cast<int>(MSG_TYPE::UPLOAD));

        buffer[part_index] = (char*) malloc (MAX_SIZE_PER_SHARD);
        fread(buffer[part_index], 1, MAX_SIZE_PER_SHARD, fp);
        LOG(INFO) << "send buffer: " << buffer[part_index] << ", part_index: " << part_index;
        string md5_val = md5(buffer[part_index]);
        
        request.set_shard(shard);
        request.set_part_index(part_index);
        request.set_part_sign(md5_val);
        request.set_file_name(file);
        request.set_file_sign(file_md5);
        request.set_usr(usr);
        request.set_body(buffer[part_index]);
        _stub->Send(&cntl, &request, &response, NULL);
        if (!cntl.Failed()) {
            LOG(INFO) << "Received response from " << cntl.remote_side()
                << " to " << cntl.local_side()
                << ": " << response.body()
                << ", res:" << response.res()
                << " latency=" << cntl.latency_us() << "us";
        } else {
            LOG(WARNING) << cntl.ErrorText();
        }
    }
    fclose(fp);
    free(buffer);
    return 0;
}




