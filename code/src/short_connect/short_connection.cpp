#include <gflags/gflags.h>
#include "short_connection.h"
#include "mpthread.h"

using namespace std;

#define UNIX_SOCK_PATH "/tmp/unix_sock"

DECLARE_int32(mpthread_num);

void mainConnectByClient(int fd, short eventType, void* arg);

ShortConnectionServer::ShortConnectionServer(char const* ip, int port) {
    _ip = ip;
    _port = port;
    _mpthread_num = FLAGS_mpthread_num;

    //init libevent
    _event_base = event_base_new();
}

void ShortConnectionServer::Run() {
    //create _main_socket and add to libevent
    this->createServer();
    LOG(INFO) << "ShortConnection createServer succ";

    //create mpthread and pipe to send client_fd
    this->createMpthread();
    LOG(INFO) << "ShortConnection createMpthread succ";
    //loop listen waitfor callback
    event_base_dispatch(_event_base);
}

void ShortConnectionServer::createServer() {
    int main_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == main_socket) {
        LOG(FATAL)<< "serverfd creade fail, err:" << strerror(errno);
    }

    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(_port);
    inet_pton(AF_INET, _ip, &saddr.sin_addr);

    if (-1 == bind(main_socket, (struct sockaddr*)&saddr, sizeof(saddr))) {
        LOG(FATAL) << "server bind fail, err:" << strerror(errno) << "port:" << _port;
    }
    int opt = 1;
    setsockopt(main_socket, SOL_SOCKET, SO_REUSEADDR,(const void *)&opt, sizeof(opt));
    if (-1 == listen(main_socket, 20)) {
        LOG(FATAL) << "server listen fail, err:" << strerror(errno);
    }

    struct event* listen_event  = event_new(_event_base, main_socket,
            EV_READ|EV_PERSIST, mainConnectByClient, (void*)this);
    if (NULL == listen_event){
        LOG(ERROR) << "listen event_new fail, err:" << strerror(errno);
    }
    if (event_add(listen_event, NULL) < 0) {
        LOG(FATAL) << "main_socket event add failed, err:" << strerror(errno);
    }
}

void ShortConnectionServer::createMpthread() {
    for (int i=0; i<_mpthread_num; ++i) {
        int pipe_fd[2];
        if (pipe(pipe_fd) < 0) {
            LOG(FATAL) << "create Mpthread pipe_fd fail, err: " << strerror(errno);
        }
        new Mpthread(pipe_fd[0]);
        _mpthread_fd_list.push_back(pipe_fd[1]);
    }
}

int ShortConnectionServer::MpthreadNum() {
    return _mpthread_num;
}

int ShortConnectionServer::GetMpthreadFd(int index) {
    return _mpthread_fd_list[index];
}

void mainConnectByClient(int fd, short eventType, void* arg) {
    ShortConnectionServer* ser_this = (ShortConnectionServer*)arg;
    struct sockaddr_in caddr;
    int len = sizeof(caddr);
    int client_fd = accept(fd, (struct sockaddr*)&caddr, (socklen_t*)&len);
    if (-1 != client_fd) {
        LOG(INFO) << "accept client, ip:" << inet_ntoa(caddr.sin_addr);
        int thread_num = ser_this->MpthreadNum();
        write(ser_this->GetMpthreadFd(rand() % thread_num),  &client_fd, sizeof(client_fd));
        LOG(INFO) << "mainConnectByClient and send fd";
    } else {
        LOG(WARNING) << "accept client failed, err:" << strerror(errno);
    }
}
