# 简介
brpc是用c++语言编写的工业级RPC框架  
http://brpc.incubator.apache.org/zh/docs/overview/

# 安装
要运行brpc需要先安装三个依赖项，按需安装
- gflags
- protobuf
- leveldb
```
# gflag
# https://github.com/gflags/gflags
git clone https://github.com/gflags/gflags.git
yum install libgflags-dev
mkdir build && cd build
cmake ..
make
```
```
# brpc
yum install -y git g++ make libssl-dev libgflags-dev libprotobuf-dev libprotoc-dev protobuf-compiler libleveldb-dev
yum install -y libsnappy-dev
yum install -y libgoogle-perftools-dev
yum install -y cmake libgtest-dev && cd /usr/src/gtest && sudo cmake . && sudo make && sudo mv libgtest* /usr/lib/ && cd -
# include 和 lib 目录按照本机地址,glog/logging.h也需要放在/usr/include下
sh config_brpc.sh --headers=/usr/include --libs=/usr --with-glog
```

# Demo
参考demo/brpc，需要先将proto编译一下 protoc echo.proto --cpp_out=./

# bvar 与 mbvar

# 扩展