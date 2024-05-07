# 简介
结构化存储日志库：github 地址：`https://github.com/google/glog`

# 安装
```
git clone https://github.com/google/glog.git
cd glog
cmake -S . -B build -G "Unix Makefiles"
cmake --build build
#在执行demo时如果找不到动态链接库需要手动把路径添加到.bashrc中
/usr/local/lib
```

# 使用demo
```
#include <glog/logging.h>
#include <sys/stat.h> //mkdir
#include <dirent.h> //opendir
int main(int argc, char *argv[])
{
    if (opendir("log")== NULL) {
        mkdir("log", 0755);
    }
    google::InitGoogleLogging(argv[0]);
    google::SetLogDestination(google::INFO, "log/glog_demo.log"); // 把日志同时记录文件，最低级别为INFO,不会创建目录

    // 通过GFLAGS来设置参数，更多选项可以在logging.cc里面查询
    // 日志等级分为INFO, WARNING, ERROR, FATAL,如果是FATAL级别这直接运行报错
    FLAGS_stderrthreshold = google::WARNING;
    FLAGS_colorlogtostderr = true;

    LOG(WARNING) << "Hello GLOG";
    // 条件输出
    LOG_IF(INFO, 20 > 10) << "20 > 10";
    google::ShutdownGoogleLogging();
}
```
# cmake
```
find_package (glog)
add_executable(glog_demo demo/glog_demo.cpp)
target_link_libraries (glog_demo glog)
```