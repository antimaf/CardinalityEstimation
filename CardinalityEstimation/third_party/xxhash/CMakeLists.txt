cmake_minimum_required(VERSION 2.8.12)
project(xxhash)

add_library(xxhash STATIC xxhash.c)
target_include_directories(xxhash PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
