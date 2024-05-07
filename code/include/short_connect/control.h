#pragma once

#include <map>
#include <iostream>
#include <jsoncpp/json/json.h>
#include <glog/logging.h>
#include <sys/socket.h> 

#include "public.h"
#include "view.h"
#include "register_view.h"

using namespace std;

void SendResponse(int fd, Json::Value& val);

class Control
{
public:
    bool Init();
    void Process(char* buff,int client_fd);
private:
    //find the view by event type
    map<int, View*> _view_map;
};
