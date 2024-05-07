#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>

using namespace std;

int calMd5(string file_name, string& s) {
    string cmd = "md5sum " + file_name;
    FILE *fp = popen(cmd.c_str(), "r");
    if (fp == NULL) {
        return -1;
    }
    char buf[33];
    int nread = fread(buf, 1, 32, fp);
    if (nread <= 0) {
        return -1;
    }
    buf[32] = '\0';
    s = buf;
    return 0;
}

int main() {
    string s_md5;
    int res = calMd5("data", s_md5);
    cout << res << endl;
    cout << s_md5 << endl;
    return 0;
}
