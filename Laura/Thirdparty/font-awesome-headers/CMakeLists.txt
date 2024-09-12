cmake_minimum_required(VERSION 3.1)
project(fontAwesome)

add_library(fontAwesome INTERFACE)


target_include_directories(fontAwesome INTERFACE include "${CMAKE_CURRENT_SOURCE_DIR}/")