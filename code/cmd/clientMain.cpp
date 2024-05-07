#include <iostream>
#include <string>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <jsoncpp/json/json.h>
#include <glog/logging.h>
#include <brpc/channel.h>
#include <glog/logging.h>

#include "brpc_client.h"
#include "gflag_def.h"
#include "public.h"

#define RESPONSE_MAX_LEN 1024
#define DEFAULT_USR "lxw01"

using namespace std;

string g_usr = DEFAULT_USR;

bool g_is_login = false;
const char* g_ser_ip = "10.0.24.3";
int g_short_conn_port = 8124;
int g_long_conn_port = 8125;

void initGlog(char* main);
void registOp();
void loginOp();
void exitOp();
void uploadOp(BrpcClient& bclient, string usr);
void downloadOp(BrpcClient& bclient, string usr);
bool sendShortReq(const char* req_msg, int& fd);
bool parseResponse(int fd, Json::Value& json_val);

void signal_send(int sig){
    cout << "server not start, exit" << endl;
}

int main(int argc, char* argv[]) {
    initGlog(argv[0]);
    signal(SIGPIPE, signal_send);

    BrpcClient bclient(string(g_ser_ip) + ":" + to_string(g_long_conn_port));
    bclient.Init();

    cout << "=====welcome to Network disk server=====" << endl;
    cout << "========================================" << endl;
    //connectServer();
    while (1) {
        if (!g_is_login) {
            cout << endl;
            cout << "========================================" << endl;
            cout << "!!!not online, please login or regist first" << endl;
            cout << "========================================" << endl;
        }
        cout << "select operation type" << endl;
        cout << "|\t0: regist\t|" << endl;
        cout << "|\t1: login\t|" << endl;
        cout << "|\t2: exit\t|" << endl;
        cout << "|\t3: share\t|" << endl;
        cout << "|\t4: delete\t|" << endl;
        cout << "|\t5: upload\t|" << endl;
        cout << "|\t6: download\t|" << endl;
        cout << "|\t7: share\t|" << endl;
        cout << "input type num: ";
        int op_type;
        cin >> op_type;
        cout << static_cast<int>(MSG_TYPE::UPLOAD) << endl;
        switch (op_type) {
            case static_cast<int>(MSG_TYPE::REGISTER):
                registOp();
                break;
            case static_cast<int>(MSG_TYPE::LOGIN):
                loginOp();
                break;
            case static_cast<int>(MSG_TYPE::EXIT):
                exitOp();
                break;
            case static_cast<int>(MSG_TYPE::UPLOAD):
                uploadOp(bclient, g_usr);
                break;
            case static_cast<int>(MSG_TYPE::DOWNLOAD):
                downloadOp(bclient, g_usr);
                break;
            default:
                cout << "Error: not support!" << endl;
        }
    }
    return 0;
}

void registOp() {
    cout << "input usr: ";
    string usr;
    cin >> usr;
    cout << "input passwd: ";
    string passwd;
    cin >> passwd;
    cout << "repeat passwd: ";
    string repeat_passwd;
    cin >> repeat_passwd;

    Json::Value req_json;
    req_json["Type"] = 0;
    req_json["Name"] = usr.c_str();
    req_json["Passwd"] = passwd.c_str();
    
    int fd;
    if (sendShortReq(req_json.toStyledString().c_str(), fd)) {
        Json::Value val;
        if (parseResponse(fd, val)) {
            cout << "Server: " << val["Response"] << endl;
        } else {
            cout << "Error: parse response json failed" << endl;
        }
    } else {
        cout << "Error: send request failed" << endl;
    }
    return;
}

void loginOp() {
    cout << "in loginOp" << endl;
    return;
}

void exitOp() {
    return;
}

void uploadOp(BrpcClient& bclient, string usr) {
    if (!g_is_login) {
        cout << "Error: not login, Login first" << endl;
    }
    string upload_file;
    cout << "file: ";
    cin >> upload_file;
    if (bclient.DoRequest(static_cast<int>(MSG_TYPE::UPLOAD), upload_file, usr) == 0) {
        cout << "upload file: " << upload_file << "success" << endl;
    } else {
        cout << "Error: upload file err" << endl;
    }
    return;
}

void downloadOp(BrpcClient& bclient, string usr) {
    return;
}

bool sendShortReq(const char* req_msg, int& fd) {
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        cout << "create socket error, err:" << strerror(errno) << endl;
        return false;
    }
    struct sockaddr_in seraddr;
    memset(&seraddr, 0, sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(g_short_conn_port);
    inet_pton(AF_INET, g_ser_ip, &seraddr.sin_addr);
    
    if (connect(fd, (struct sockaddr*)&seraddr, sizeof(struct sockaddr)) < 0) {
        cout << "Error: connect failed" << strerror(errno) << endl;
        close(fd);
        return false;
    }

    if (send(fd, req_msg, strlen(req_msg), 0) < 0) {
        cout << "Error: send msg error: " << strerror(errno) << endl;
        close(fd);
        return false;
    }
    cout << "send: " << req_msg << endl;
    return true;
}

bool parseResponse(int fd, Json::Value& json_val) {
    char res_buf[RESPONSE_MAX_LEN];
    int recv_len;
    if ((recv_len = recv(fd, res_buf, RESPONSE_MAX_LEN,0)) == -1) {
        cout << "Error: recv response error, err: " << strerror(errno) << endl;
        close(fd);
        return false;
    }
    Json::Reader read;
    if (!read.parse(res_buf, json_val)) {
        cout << "Error: parse response failed, res_buf: " << res_buf << endl;
        close(fd);
        return false;
    }
    return true;
}

void initGlog(char* main) {
    google::InitGoogleLogging(main);
    FLAGS_stderrthreshold = google::INFO;
    FLAGS_colorlogtostderr = true;
}
