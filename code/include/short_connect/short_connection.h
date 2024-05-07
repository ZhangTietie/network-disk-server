#pragma once

#include <iostream>
#include <event.h> //libevent
#include <glog/logging.h>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include<stdlib.h> //rand
#include<arpa/inet.h>
#include <unistd.h> //pipe

#include "public.h"

using namespace std;

class ShortConnectionServer {
public:
    //初始化成员变量
    ShortConnectionServer(char const* ip, int port);

    /*
       1. 创建_main_socket并使用libevent注册监听客户端连接事件
       2. 针对每个线程创建一个无名管道，主线程使用该管道与支线程进行通信，其中0端读 1端写
    */
    void Run();
    int MpthreadNum();
    int GetMpthreadFd(int index);
private:
    void createServer();
    void createMpthread();

    int _port;
    const char* _ip;
    int _mpthread_num;
    vector<int> _mpthread_fd_list;
    struct event_base* _event_base;

    friend void recvClientMsg(int fd, short event, void *arg);
};

