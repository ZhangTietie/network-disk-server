#include <iostream>
#include <glog/logging.h>
#include <pthread.h>
#include <gflags/gflags.h>

#include "short_connection.h"
#include "long_connection.h"
#include "logger.h"
#include "gflag_def.h"

DECLARE_string(ip);
DECLARE_int32(short_port);
DECLARE_int32(long_port);

using namespace std;

void* short_ser_thread_run(void* arg) {
    ShortConnectionServer* pshort_ser = (ShortConnectionServer*) arg;
    pshort_ser->Run();
    return nullptr;
}

class serverMain {
public:
    serverMain(const char* ip, int short_port, int long_port, char* main) :
        _ip(ip), _short_port(short_port), _long_port(long_port), _main(main) {}

    void runUntilAskedToQuit() {
        //初始化log
        _log.Init(_main);
        //负责管理短链接的客户端请求，如注册、登录、退出等消息
        ShortConnectionServer short_ser(_ip, _short_port);
        pthread_t id;
        pthread_create(&id, NULL, short_ser_thread_run, (void*)&short_ser);

        //负责管理长连接的客户端请求 -- 上传文件
        LongConnectionServer long_ser(_ip, _long_port);
        long_ser.Run();
    }
private:
    Logger _log;

    const char* _ip;
    int _short_port;
    int _long_port;
    char* _main;
};

int main(int argc, char* argv[]) {
    google::ParseCommandLineFlags(&argc, &argv, true);

    string ip = FLAGS_ip;
    serverMain ser(ip.c_str(), FLAGS_short_port, FLAGS_long_port, argv[0]);

    ser.runUntilAskedToQuit();
    LOG(WARNING) << "serverMain quit";
    google::ShutdownGoogleLogging();

    return 0;
}


