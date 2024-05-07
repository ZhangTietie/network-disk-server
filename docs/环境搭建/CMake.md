# 简介
CMake支持跨系统的项目构建，即在Linux下可以生成makefile，写法相较makefile更加简单。

# 函数用法
|  函数名   | 含义  |
|  ----  | ----  |
| PROJECT(< name >)  | 项目名 |
| CMAKE_MINIMUM_REQUIRED("VERSION 2.6")   | cmake 2.6版本 |
| AUX_SOURCE_DIRECTORY(< dir > < variable >)|  将 dir 目录下所有的文件名赋值给 variable 对应的变量 |
| ADD_SUBDIRECTORY(< dir >) | 为项目增加 dir 目录 |
| SET( < variable > <file1 file2 file3>) | 创建一个变量，包括了 file1，file2， file3 文件 |
| ADD_EXECUTABLE(< app > < variable >) | 由指定变量对应的文件生成可执行文件 |
| ？？？？？ | ？？？？|
| include_directories |  |
| link_directories |  |
| add_compile_options |  |
| target_link_libraries |  |
# Demo

