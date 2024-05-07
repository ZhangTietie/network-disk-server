#include "public.h"

#define SHELLOUT_MAX_LEN 256

#include <iostream>
using namespace std;

int ShellOut(string cmd, string& res) {
    cout << "run Shellout cmd:" <<cmd << endl;
    FILE *fp = popen(cmd.c_str(), "r");
    if (fp == NULL) {
        return -1;
    }
    char out[SHELLOUT_MAX_LEN];
    int write_index = 0;
    int nread = 0;
    do {
        nread = fread(out + write_index, 1, 10, fp);
        write_index += nread;
    }while(nread);
    res = string(out);
    pclose(fp);
    return 0;
}

int Md5Sum(string file_path, string& md5) {
    string cmd = "md5sum " + file_path;
    string res;
    if (ShellOut(cmd, res) != 0) {
        return -1;
    }
    md5 = res.substr(0, 32);
    return 0;
}
