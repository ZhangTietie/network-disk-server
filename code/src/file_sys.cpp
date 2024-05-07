#include "file_sys.h"
#include <iostream>
using namespace std;

#define  WRITE_LEN 4096

int FileSys::Mkdir(string dirName) {
    if (mkdir(dirName.c_str(), 0755) == -1 && errno != EEXIST) {
        return -1;
    }
    return 0;
}

bool FileSys::ExistDir(string dirName) {
    if (opendir(dirName.c_str()) != NULL) {
        return true;
    } else {
        return false;
    }
}

bool FileSys::ExistFile(string file_path) {
    return access(file_path.c_str(), F_OK) == 0;
} 

int FileSys::LsDir(string dirName, vector<string>& res) {
    cout << "dirName:" << dirName << endl;
    DIR* p_dir = opendir(dirName.c_str());
    if (p_dir == NULL) {
        return -1;
    }
    struct dirent* p_dirent;
    while(p_dirent = readdir(p_dir)) {
        cout << p_dirent->d_name[0] << endl;
        if (p_dirent->d_name[0] != '.') {
            res.push_back(string(p_dirent->d_name));
        }
    }
    return 0;
}

int FileSys::Write(const string& str, string path) {
    const char* s = str.c_str();
    int fd = open(path.c_str(), O_RDWR | O_CREAT, S_IREAD|S_IWRITE);
    if (fd == -1) {
        return -1;
    }
    int len = strlen(s);
    while (len > WRITE_LEN) {
        write(fd, s, WRITE_LEN);
        len -= WRITE_LEN;
    }
    write(fd, s, len);
    close(fd);
    return 0;
}

int FileSys::DeleteFile(string path) {
    return remove(path.c_str());
}

int FileSys::Link(string src_path, string dst_path) {
    if (symlink(src_path.c_str(), dst_path.c_str()) != 0 && errno != EEXIST) {
        cout << "link failed" << endl;
        return -1;
    }
    cout << "link success" << endl;
    return 0;
}







