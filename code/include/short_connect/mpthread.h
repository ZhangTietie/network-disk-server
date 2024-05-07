#pragma once

#include <iostream>
#include <event.h> //libevent
#include <glog/logging.h>
#include <event.h>
#include <pthread.h>
#include <error.h>
#include <errno.h>
#include <glog/logging.h>

#include "control.h"

using namespace std;

class Mpthread {
public:
    //创建处理消息的支线程
    Mpthread(int read_fd);
    int ReadFd();
private:
    int _read_fd;
    struct event_base* _event_base;

    //通过将消息发送到control选择对应的视图
    Control _control;

    //pthread 执行的函数
    friend void* thread_run(void *arg);
    //通过read_fd接收从主线程发来的clientfdi, 并读取客户端的消息
    friend void recvClientMsg(int fd,short event,void *arg);
};
