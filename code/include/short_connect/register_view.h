#pragma once
#include <iostream>
#include <jsoncpp/json/json.h>
#include <glog/logging.h>
#include <string>

#include"view.h"

using namespace std;

class RegisterView :public View {
public:
    void Process(Json::Value val,int client_fd) override;

private:
    int usrNotExist(string usr);
    int insertUsr(string usr, string passwd);
    int createUsrDir(string usr);
};
