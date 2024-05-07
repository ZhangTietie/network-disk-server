#pragma once

#include <gflags/gflags.h>

//ServerMain
DEFINE_string(ip, "10.0.24.3", "server default ip");
DEFINE_int32(short_port, 8124, "TCP Port of short connection");
DEFINE_int32(long_port, 8125, "BRPC Port of long connection");

DEFINE_string(mysql_ip, "127.0.0.1", "mysql default ip");
DEFINE_int32(mysql_port, 3306, "mysql default port");
DEFINE_string(mysql_database, "lxw01", "mysql default database");
DEFINE_string(mysql_usr, "root", "mysql default usr");
DEFINE_string(mysql_passwd, "root", "mysql default passwd");

DEFINE_string(usr_dir, "/home/ubuntu/network-disk-server/server_data/usr/", "default usr dir");
DEFINE_string(files_dir, "/home/ubuntu/network-disk-server/server_data/files/", "default files dir");
DEFINE_string(client_files_dir, "/home/ubuntu/network-disk-server/client_data/files/", "default client files dir");

//ShortConnectionServer
DEFINE_int32(mpthread_num, 3, "num of pthread for deal with short connection");

//LongConnectionServer
DEFINE_string(protocol, "baidu_std", "Protocol type. Defined in src/brpc/options.proto");
DEFINE_string(connection_type, "", "Connection type. Available values: single, pooled, short");
DEFINE_int32(timeout_ms, 100, "RPC timeout in milliseconds");
DEFINE_int32(max_retry, 3, "Max retries(not including the first RPC)");
DEFINE_string(load_balancer, "", "The algorithm for load balancing");
DEFINE_int32(idle_timeout_s, -1, "Connection will be closed if there is no "
             "read/write operations during the last `idle_timeout_s'");
