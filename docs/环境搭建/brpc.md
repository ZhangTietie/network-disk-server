# ���
brpc����c++���Ա�д�Ĺ�ҵ��RPC���  
http://brpc.incubator.apache.org/zh/docs/overview/

# ��װ
Ҫ����brpc��Ҫ�Ȱ�װ������������谲װ
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
# include �� lib Ŀ¼���ձ�����ַ,glog/logging.hҲ��Ҫ����/usr/include��
sh config_brpc.sh --headers=/usr/include --libs=/usr --with-glog
```

# Demo
�ο�demo/brpc����Ҫ�Ƚ�proto����һ�� protoc echo.proto --cpp_out=./

# bvar �� mbvar

# ��չ