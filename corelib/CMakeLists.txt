cmake_minimum_required(VERSION 3.16)
project(corelib)

file(GLOB_RECURSE SOURCES "*.cc")

add_library(corelib SHARED ${SOURCES})
target_link_libraries(corelib enet)