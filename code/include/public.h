#pragma once

#include <unistd.h>
#include <string.h>
#include <string>

using namespace std;

enum class MSG_TYPE
{
    REGISTER,
    LOGIN,
    EXIT,
    SHARE,
    DELETE,
    UPLOAD,
    DOWNLOAD,
    SHOW
};

int ShellOut(string cmd, string& res);

int Md5Sum(string file_path, string& md5);
