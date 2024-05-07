#include "upload_rpc.h"

DECLARE_string(usr_dir);
DECLARE_string(files_dir);

void UploadRpc::Init(Mmysql& sql, pthread_mutex_t& sql_mutex) {
    _sql = sql;
    _sql_mutex = sql_mutex;
}

void UploadRpc::Do(const example::NetDiskRequest* request, example::NetDiskResponse* response) {
    LOG(INFO) << "client start do upload";
    string usr = request->usr();
    string file_sign = request->file_sign();
    
    //创建用户files目录
    if (_file_sys.Mkdir(FLAGS_usr_dir + usr + "/files") != 0) {
        LOG(ERROR) << "create usr files dir failed, err: " << strerror(errno)
            << ", dir: " << FLAGS_usr_dir + usr + "files";
        response->set_res(false);
        response->set_body("create usr files dir failed");
        return;
    }

    LOG(INFO) << "check file exist or not";
    //检查文件md5是否存在，如果存在则进行秒传，不存在则继续上传逻辑
    string file_dir = FLAGS_files_dir + file_sign;
    if (_file_sys.ExistDir(file_dir)) {
        if (linkToUsr(file_dir, FLAGS_usr_dir + usr + "/files/" + file_sign, file_sign, request->file_name()) != 0) {
            LOG(ERROR) << "Link failed, err: " << strerror(errno);
            response->set_res(false);
            response->set_body("speed upload link failed");
            return;
        }
        response->set_res(true);
        response->set_body("file already in server, speed");
        return;
    }

    string usr_upload_dir = FLAGS_usr_dir + usr + "/tmp/";
    //创建用户tmp目录下，md5命名的目录
    if (_file_sys.Mkdir(FLAGS_usr_dir + usr) || _file_sys.Mkdir(usr_upload_dir) != 0 ||_file_sys.Mkdir(usr_upload_dir + file_sign + '/') != 0) {
        LOG(ERROR) << "create usr tmp dir failed, err: " << strerror(errno)
            << ", dir: " << usr_upload_dir + file_sign;
        response->set_res(false);
        response->set_body("create usr dir or usr file dir failed");
        return;
    }    
    
    //查看当前分片是否存在，将当前分片写入
    string file_path = usr_upload_dir + file_sign + '/' + to_string(request->part_index());
    LOG(INFO) << "check file shard exist, file_path: " << file_path;
    if (_file_sys.ExistFile(file_path)){
        string cur_md5;
        if (Md5Sum(file_path, cur_md5) != 0) {
            LOG(ERROR) << "cal md5 failed";
            response->set_res(false);
            response->set_body("cal md5 failed");
            return;
        }
        if (cur_md5 == request->part_sign()) {
            LOG(INFO) << "part is existed, skip upload"
                << ", file_name:" << request->file_name()
                << ", part_index: " << request->part_index();
            response->set_body("this part already exist");
        } else {
            _file_sys.DeleteFile(file_path);
        }
    }

    LOG(INFO) << "request body: " << request->body();
    if (_file_sys.Write(request->body(), file_path) != 0) {
        LOG(ERROR) << "write file failed, err: " << strerror(errno)
            << ", file_name: " << request->file_name()
            << ", file_path: " << file_path;
        response->set_res(false);
        response->set_body("write file failed");
        return;
    }

    string cur_md5;
    if (Md5Sum(file_path, cur_md5) != 0) {
        LOG(ERROR) << "cal md5 failed";
        response->set_res(false);
        response->set_body("cal md5 failed");
        return;
    }
    if (cur_md5 != request->part_sign()) {
        LOG(ERROR) << "part sign check ERROR, request sign: " << request->part_sign()
            << ", cur sign: " << cur_md5;
        response->set_res(false);
        response->set_body("part sign check error");
        return;
    }

    //检查所有分片是否已经到达
    //如果到达，mv到根存储目录，ln 到用户目录，sql加md5到文件名的映射，引用计数加1，返回上传成功
    if (allPartArrived(usr_upload_dir + file_sign, request->shard())) {
        string shell_res;
        string cmd = "mv " + usr_upload_dir + file_sign  + " " + FLAGS_files_dir;
        ShellOut(cmd, shell_res);
        if (shell_res.size() != 0) {
            LOG(ERROR) << "mv failed, err: " << strerror(errno) << ", shell_res:" << shell_res;
            response->set_res(false);
            response->set_body("mv failed");
            return;
        }
        if (linkToUsr(file_dir,  FLAGS_usr_dir + usr + "/files/" + file_sign, file_sign, request->file_name()) != 0){
            LOG(ERROR) << "Link failed, err: " << strerror(errno);
            response->set_res(false);
            response->set_body("link failed");
            return;
        }
    }

    response->set_res(true);
    response->set_body("success");
    LOG(INFO) << "upload file succ, file_name: " << request->file_name()
        << ", part_index: " << request->part_index()
        << ", shard: " << request->shard()
        << ", usr: " << usr;
}

bool UploadRpc::allPartArrived(string dir, int shard) {
    vector<string> part_num;
    if (_file_sys.LsDir(dir, part_num) != 0) {
        LOG(ERROR) << "LsDir failed, err: " << strerror(errno)
            << "dir: " << dir; 
    }
    LOG(INFO) << "in allPartArrived shard: " << shard <<  ", arrive num: " << part_num.size(); 
    return shard == part_num.size();
}

int UploadRpc::addSignMap(string sign, string fileName) {
    string cmd = "INSERT INTO SignMap (sign, fileName, reference) VALUES('" + sign + "','" + fileName + "',1);";
    pthread_mutex_lock(&_sql_mutex);
    if (_sql.RunQuery(cmd)) {
        LOG(ERROR) << "insert SignMap failed, cmd: " << cmd;
        pthread_mutex_unlock(&_sql_mutex);
        return -1;
    }
    pthread_mutex_unlock(&_sql_mutex);
    return 0;
}

int UploadRpc::linkToUsr(string src, string dst, string sign, string file_name) {
    LOG(INFO) << "Link to Usr, src: " << src << ", dst: " << dst;
    if (_file_sys.Link(src, dst) != 0) {
        LOG(ERROR) << "Link failed, err: " << strerror(errno);
        return -1;
    }
    //判断是新增文件还是集群中已有文件
    string cmd = "SELECT * FROM SignMap where sign = '" + sign + "'";
    if (_sql.RunQuery(cmd)) {
        LOG(ERROR) << "search file sign fail, cmd: " << cmd;
        return -1;
    }
    if (_sql.FetchRes() == NULL) {
        if (addSignMap(sign, file_name) != 0) {
            LOG(ERROR) << "addSignMap failed,  err: " << strerror(errno);
            return -1;
        }
    } else {
        string cmd = "UPDATE SignMap Set reference = reference + 1 where sign = '" + sign + "'";
        pthread_mutex_lock(&_sql_mutex);
        if (_sql.RunQuery(cmd)) {
            LOG(ERROR) << "insert SignMap failed, cmd: " << cmd;
            pthread_mutex_unlock(&_sql_mutex);
            return -1;
        }
        pthread_mutex_unlock(&_sql_mutex);
    }
    return 0;
}

