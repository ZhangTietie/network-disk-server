#pragma once
#include <string>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <vector>
#include <string.h>

using namespace std;

class FileSys {
public:
    bool ExistDir(string dirName);
    bool ExistFile(string filePath);
    int Mkdir(string dirName);
    int LsDir(string dirName, vector<string>& res);
    int Write(const string& str, string path);
    int DeleteFile(string path);
    int Link(string src_path, string dst_path);
};
