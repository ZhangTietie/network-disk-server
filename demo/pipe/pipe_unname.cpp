#include<unistd.h>
#include<iostream>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <event.h>
using namespace std;

void recvClientMsg(int fd, short event, void *arg) {
    int client;
    if (read(fd, &client, sizeof(client)) < 0) {
        cout << "read error" << endl;
    } else {
        cout << "i am child: " << client << endl;
    }
}

void* thread_run(void* arg) {
    int fd = *(int*)arg;

    struct event_base* event_base = event_base_new();
    struct event* listen_event = event_new(event_base, fd, EV_READ|EV_PERSIST, recvClientMsg, NULL);
    event_add(listen_event,NULL);
    event_base_dispatch(event_base);
}


int main () {

    int fd[2];
    if (pipe(fd) < 0) {
        cout << "create pipe failed" << endl;
    }

    /*
    int pid = fork();
    if (pid < 0) {
        cout << "fork error" << endl;
    } else if (pid == 0) {
        int client;
        if (read(fd[0], &client, sizeof(client)) < 0) {
            cout << "read error" << endl;
        } else {
            cout << "i am child: " << client << endl;
        }
    } else {
        int c = 888;
        if (write(fd[1], &c, sizeof(c)) < 0) {
            cout << "write error:" << strerror(errno) <<endl;
        } else {
            cout << "i am father: " << c << endl;
        }
    }
    */

    pthread_t id;
    pthread_create(&id, NULL, thread_run, (void*)&(fd[0]));

    int c = 888;
    if (write(fd[1], &c, sizeof(c)) < 0) {
        cout << "write error:" << strerror(errno) <<endl;
    } else {
        cout << "i am father: " << c << endl;
    }

    for(;;) {
    }
    
    return 0;
}
