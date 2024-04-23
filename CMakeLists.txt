cmake_minimum_required(VERSION 3.20)
project(bitmap)
add_library(bitmap STATIC code/BMPfile.cpp)
add_subdirectory(tests)