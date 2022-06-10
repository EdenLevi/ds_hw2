cmake_minimum_required(VERSION 3.21)
project(ds_hw2)

set(CMAKE_CXX_STANDARD 11)

add_executable(ds_hw2 main.cpp Hashtable.h rankTree.cpp rankTree.h tree.h tree.cpp classes.h)
