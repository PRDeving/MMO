cmake_minimum_required(VERSION 3.16)
project(client)

include_directories(${mmo_SOURCE_DIR}/include)
include_directories(${corelib_SOURCE_DIR}/include)
file(GLOB_RECURSE SOURCES "*.cc")

add_executable(client ${SOURCES})
target_link_libraries(client PRIVATE corelib)