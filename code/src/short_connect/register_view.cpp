#include<iostream>
#include<jsoncpp/json/json.h>
#include<errno.h>
#include <gflags/gflags.h>

#include "register_view.h"
#include "control.h" //SendResponse

DECLARE_string(usr_dir);

using namespace std;

void RegisterView::Process(Json::Value req_val, int client_fd) {
    LOG(INFO) << "start process RegisterView";
    Json::Value res;
    string response_msg;
    string usr = req_val["Name"].asString();
    string passwd = req_val["Passwd"].asString();
    int rt;
    do {
        rt = usrNotExist(usr);
        if (rt == -1) {
            response_msg = "Fail: query failed";
            break;
        } else if (!rt) {
            cout << "ztz is exist" << endl;
            response_msg = "Fail: usr is existed, usr: " + usr;
            break;
        } 

        rt = insertUsr(usr, passwd);
        if (rt == -1) {
            response_msg = "Fail: insert Usr table failed";
            break;
        }

        rt = createUsrDir(usr);
        if (rt == -1) {
            response_msg = "Fail: create usr dir failed";
            break;
        }
        response_msg = "Success: regist usr " + usr;

    } while (0);
    res["Response"] = response_msg;
    SendResponse(client_fd, res);
}

int RegisterView::usrNotExist(string usr) {
    string cmd = "SELECT uid FROM Usr where name = '" + usr + "';";
    if (_sql.RunQuery(cmd)) {
        LOG(ERROR) << "query name in table Usr fail, cmd: " << cmd;
        return -1;
    }
    if (_sql.FetchRes() == NULL) {
        cout << "not exist" << endl;
        return true;
    } else {
        return false;
    }
}

int RegisterView::insertUsr(string usr, string passwd) {
    string cmd = "INSERT INTO Usr (name, passwd) VALUES('" + usr + "','" + passwd + "');";
    if (_sql.RunQuery(cmd)) {
        LOG(ERROR) << "insert new usr in table Usr failed, cmd: "<< cmd;
        return -1;
    }
    return true;
}

int RegisterView::createUsrDir(string usr) {
    string path = FLAGS_usr_dir + usr;
    if (this->getFileSys().Mkdir(path) == -1) {
        LOG(ERROR) << "create usr dir failed, dir path: " << path;
        return -1;
    } else {
        return true;
    }
}
