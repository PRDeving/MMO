cmake_minimum_required(VERSION 3.16)
project(game-server)

include_directories(${mmo_SOURCE_DIR}/src/include)
include_directories(${corelib_SOURCE_DIR}/include)
file(GLOB_RECURSE SOURCES "*.cc")

add_executable(game-server ${SOURCES})
target_link_libraries(game-server PRIVATE corelib)