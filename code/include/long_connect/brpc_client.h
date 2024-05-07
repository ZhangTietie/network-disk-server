#pragma once
#include <iostream>
#include <string>
#include <brpc/channel.h>

#include "NetDisk.pb.h"
#include "public.h"

using namespace std;

class BrpcClient {
public:
    BrpcClient(string server):_server(server) {}
    int Init();
    int DoRequest(const int type, string file, string usr);
private:
    int doUploadFile(string file, string usr);

    string _server;
    example::NetDiskService_Stub* _stub;
};
