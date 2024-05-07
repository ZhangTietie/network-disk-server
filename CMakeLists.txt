cmake_minimum_required(VERSION 3.8)
project(netDiskServer)
find_package (glog)

# build glog_demo
#add_executable(glog_demo demo/glog_demo.cpp)
#target_link_libraries (glog_demo glog)


# build brpc demo

#set (INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src
#                    ${CMAKE_CURRENT_SOURCE_DIR}/include
#                    /usr/include)
#set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib
#           /usr/local/lib)
#
#add_compile_options(-std=c++11 -g)
#
#include_directories(${INCLUDE_DIR})
#link_directories(${LIB_DIR})
#
#add_executable(client demo/brpc/client.cpp demo/brpc/echo.pb.h demo/brpc/echo.pb.cc)
#add_executable(server demo/brpc/server.cpp demo/brpc/echo.pb.h demo/brpc/echo.pb.cc)
#
#target_link_libraries(client
#    -lgflags
#    -lbrpc
#    -lprotobuf
#    -lleveldb
#    -lglog
#    )

#target_link_libraries(server
#    -lgflags
#    -lbrpc
#    -lprotobuf
#   -lleveldb
#    -lglog
#    )
#set_target_properties(client PROPERTIES LINK_FLAGS -Wl,-rpath=/usr/lib)

# build project
set (INCLUDE_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/code/proto
        ${CMAKE_CURRENT_SOURCE_DIR}/code/include
        ${CMAKE_CURRENT_SOURCE_DIR}/code/include/long_connect
        ${CMAKE_CURRENT_SOURCE_DIR}/code/include/short_connect
        /usr/include
)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib
        /usr/lib
)

file(GLOB_RECURSE source code/src/*.cpp)
add_compile_options(-std=c++11 -g)


include_directories(${INCLUDE_DIR})
link_directories(${LIB_DIR})

add_executable(serverMain
        code/cmd/serverMain.cpp
        code/proto/NetDisk.pb.h
        code/proto/NetDisk.pb.cc
        ${source}
)

add_executable(clientMain
        code/cmd/clientMain.cpp
        code/src/long_connect/brpc_client.cpp
        code/proto/NetDisk.pb.cc
        code/proto/NetDisk.pb.h
        code/src/md5.cpp
        code/src/public.cpp
)

target_link_libraries(clientMain
    -ljsoncpp
    -lbrpc
    -lgflags
    -lprotobuf
    -lleveldb
    -lglog
)

target_link_libraries(serverMain
    -lgflags
    -lbrpc
    -lprotobuf
    -lleveldb
    -lglog
    -lpthread
    -levent
    -ljsoncpp
    -lmysqlclient
)
