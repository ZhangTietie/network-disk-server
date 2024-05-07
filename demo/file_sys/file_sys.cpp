#include <iostream>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <vector>

using namespace std;

int ExistDir(string dirName) {
    if (opendir(dirName.c_str()) != NULL) {
        return true;
    } else {
        return false;
    }
}

int Mkdir(string dirName) {
    if (mkdir(dirName.c_str(), 0755) == -1 && errno != EEXIST) {
        return -1;
    }
    return 0;
    //return mkdir(dddirName.c_str(), 0755);
}

int Write(string str, string path) {
    const char* s = str.c_str();
    int len = strlen(s);
    int fd = open(path.c_str(), O_RDWR | O_CREAT, S_IREAD|S_IWRITE);
    if (fd == -1) {
        cout << "open failed, err:" << strerror(errno) << endl;
        return -1;
    }
    cout << "lastLen: " << strlen(s) << endl;
    while (len > 10) {
        write(fd, s, 10);
        len -= 10;
    }
    cout << "write len " << len<< endl;
    write(fd, s, len);

    close(fd);
    /*
    int n = write(fd, s, 2);
    cout << "n: " << n << endl;
    n = write(fd, s+n, 2);
    cout << "n: " << n << endl;
    */
    return 0;
}

int ShellOut(string cmd, string& res) {
    FILE *fp = popen(cmd.c_str(), "r");
    if (fp == NULL) {
        cout << "ShellOut failed" << endl;
        cout << errno << endl;
        return -1;
    }
    char buf[512];
    int write_index = 0;
    int nread = 0;
    
    cout << "111" << endl;
    do {
        nread = fread(buf+write_index, 1, 10, fp);
        write_index += nread;
    }while(nread);
    cout << "buf: " << buf << endl;
    res = string(buf);
    cout << "res: " << res << endl;
    pclose(fp);
    return 0;
}

int DeleteFile(string path) {
    return remove(path.c_str());
}

int LsDir(string dirName, vector<string>& res) {
    DIR* p_dir = opendir(dirName.c_str());
    if (p_dir == NULL) {
        return -1;
    }
    struct dirent* p_dirent;
    while(p_dirent = readdir(p_dir)) {
        cout << "name: " << p_dirent->d_name << endl;
        if (p_dirent->d_name[0] != '.') {
            res.push_back(string(p_dirent->d_name));
        }
    }
    return 0;
}

int Link(string old_path, string new_path) {
    return symlink(old_path.c_str(), new_path.c_str());
}

int main() {
    
    //cout << ExistDir("/home/ubuntu/network-disk-server/server_data/usr") << endl;
    //cout << Mkdir("/home/ubuntu/network-disk-server/server_data/usr/lxw01") << endl;
    //cout << ExistDir("testDir") << endl;
    
    //Write("aaaaaa", "0");
    /*
    const char* write_string = "12345678910";
    cout << Write(string(write_string), "testDir/write_test.txt") << endl;
    */
    //string res;
    //ShellOut("mv testDir/write_test.txt testDir/write_test.bak.txt 2>&1", res);
    //ShellOut("md5sum /home/ubuntu/network-disk-server/server_data/usr/lxw01/tmp/b1ffb6b5d22cd9f210fbc8b7fdaf0e19/0", res);
    //cout << "res: " << res <<endl;
    //cout << DeleteFile("testDir/write_test.bak.txt") << endl;
    
    //vector<string> res;
    //LsDir("./testDir.bak", res);
    //for (int i = 0; i < res.size(); ++i) {
    //    cout << "file: " << res[i] << endl;
    //}
    
    
    cout << Link("/home/ubuntu/network-disk-server/server_data/files/47bce5c74f589f4867dbd57e9ca9f808",
                 "/home/ubuntu/network-disk-server/server_data/usr/lxw01/files/47bce5c74f589f4867dbd57e9ca9f808") << endl;
    cout << errno << endl;
    cout << EEXIST << endl;
    cout << (errno == EEXIST) << endl;
    /*
    cout << Link("/home/ubuntu/network-disk-server/demo/file_sys/testDir",
                 "/home/ubuntu/network-disk-server/demo/file_sys/testDir.bak") << endl;
    cout << errno << endl;
    */
    return 0;
}
