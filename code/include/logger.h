#pragma once

#include <glog/logging.h>
#include <sys/stat.h>
#include <dirent.h>

class Logger {
public:
    void Init(char* main) {
        if (opendir("log") == NULL) {
            mkdir("log", 0755);
        }
        google::InitGoogleLogging(main);
        // 把日志同时记录文件，最低级别为INFO
        google::SetLogDestination(google::INFO, "./log/server.log");
        // 通过GFLAGS来设置参数，更多选项可以在logging.cc里面查询
        // 日志等级分为INFO, WARNING, ERROR, FATAL,如果是FATAL级别这直接运行报错
        FLAGS_stderrthreshold = google::INFO;
        FLAGS_colorlogtostderr = true;
        LOG(INFO) << "SerLog init succ";
    }
};
