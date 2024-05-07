
#include<jsoncpp/json/json.h>
#include <iostream>

using namespace std;

int main() {

    const char* buf = "{\"a\": 1}";


    Json::Value val;
    Json::Reader read;
    if ( -1 == read.parse(buf, val)){
        cout << "parse json failed, buf: " << buf << endl;
    }
    cout << val["a"] << endl;

    val["b"] = 2;
    cout << val.toStyledString().c_str() << endl;
    //toStyledString: json=>string
    //c_str(): string=>const char*

    const char* p = val.toStyledString().c_str();
    char next[100] = {0};
    strcpy(next, p);

    cout << p[0] << endl;

    //cout << p << endl;
    //cout << buf << endl;
    return 0;
}
