#include "control.h"

using namespace std;

bool Control::Init() {
    View* vregister = new RegisterView();
    if (vregister->Init()) {
        LOG(FATAL) << "RegisterView init failed";
    }
    //_view_map.insert(make_pair<int, View*>(static_cast<int>(MSG_TYPE::MSG_TYPE_REGISTER), vregister));
    _view_map.insert(make_pair(static_cast<int>(MSG_TYPE::REGISTER), vregister));
    return true;
}

void SendResponse(int fd, Json::Value& val) {
    string res = val.toStyledString();
    LOG(INFO) <<  "response client, res: " << res; 
    if (send(fd, res.c_str(), res.size(), 0) < 0) {
        LOG(WARNING) << "send response failed, client_fd: " << fd
            << ", error: " << strerror(errno);
    }
    LOG(INFO) <<  "send reponse succ";
}

void Control::Process(char* msg, int client_fd)
{
    cout << "ztz process fd: " << client_fd << endl;
    //construct json bag
    Json::Value json_val;
    Json::Reader read;
    if (!read.parse(msg, json_val)){
        LOG(WARNING) << "parse client msg failed, err: " << strerror(errno);
        json_val["Response"] = "parse msg failed";
        SendResponse(client_fd, json_val);
        return;
    }

    //find the aim view
    //use view's process and responce
    if(json_val["TYPE"].asInt() == static_cast<int>(MSG_TYPE::REGISTER) ||
            json_val["TYPE"].asInt() == static_cast<int>(MSG_TYPE::LOGIN) ||
            json_val["TYPE"].asInt() == static_cast<int>(MSG_TYPE::EXIT)) {
        _view_map[json_val["TYPE"].asInt()]->Process(json_val, client_fd);
    } else {
        json_val["Response"] = "error command!";
        SendResponse(client_fd, json_val);
    }
}
