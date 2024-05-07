

#include "mpthread.h"

#define RECV_BUF_SIZE 512

using namespace std;

void* thread_run(void* arg);
void recvClientMsg(int fd, short event, void* arg);

Mpthread::Mpthread(int read_fd) {
    //init control
    if (!_control.Init()) {
        LOG(FATAL) << "control init failed" << endl;
    }

    _read_fd = read_fd;

    //开始创建pthread
    pthread_t id;
    pthread_create(&id, NULL, thread_run, (void*)this);
}

int Mpthread::ReadFd() {
    return _read_fd;
}

void* thread_run(void* arg) {
    Mpthread* mthis = (Mpthread*)arg;

    //监听unix_fd，如果主线程发送了clent_fd，则接收
    mthis->_event_base = event_base_new();

    struct event* listen_event = event_new(mthis->_event_base, mthis->ReadFd(), EV_READ|EV_PERSIST, recvClientMsg, mthis);
    if (NULL == listen_event) {
        LOG(FATAL) << "mpthread listen _unix_fd failed, err:" << errno;
    }
    event_add(listen_event, NULL);
    LOG(INFO) << "Mpthread start work";
    event_base_dispatch(mthis->_event_base);
    return nullptr;
}

void recvClientMsg(int fd, short event, void* arg) {
    Mpthread* mthis = (Mpthread*)arg;
    
    //接收client_fd
    int client_fd;
    if (read(fd, &client_fd, sizeof(client_fd)) < 0) {
        LOG(WARNING) << "get client_fd failed, err: " << strerror(errno);
    }

    //接收client_fd发送的消息
    char buf[RECV_BUF_SIZE] = {0};
    if(recv(client_fd, buf, RECV_BUF_SIZE,0) <= 0){
        LOG(WARNING) << "recv client failed, err: " << strerror(errno);
    }
    LOG(INFO) << "recv client msg: " << buf;
    
    //send(client_fd, "aaa", 3, 0);
    
    //send to Control process
    mthis->_control.Process(buf, client_fd);
    close(client_fd);
}
