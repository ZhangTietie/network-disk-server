# ���
�ṹ���洢��־�⣺github ��ַ��`https://github.com/google/glog`

# ��װ
```
git clone https://github.com/google/glog.git
cd glog
cmake -S . -B build -G "Unix Makefiles"
cmake --build build
#��ִ��demoʱ����Ҳ�����̬���ӿ���Ҫ�ֶ���·����ӵ�.bashrc��
/usr/local/lib
```

# ʹ��demo
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
    google::SetLogDestination(google::INFO, "log/glog_demo.log"); // ����־ͬʱ��¼�ļ�����ͼ���ΪINFO,���ᴴ��Ŀ¼

    // ͨ��GFLAGS�����ò���������ѡ�������logging.cc�����ѯ
    // ��־�ȼ���ΪINFO, WARNING, ERROR, FATAL,�����FATAL������ֱ�����б���
    FLAGS_stderrthreshold = google::WARNING;
    FLAGS_colorlogtostderr = true;

    LOG(WARNING) << "Hello GLOG";
    // �������
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