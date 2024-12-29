cmake_minimum_required(VERSION 3.16)
project(mmo)
set (CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)

add_subdirectory(external)
add_subdirectory(libs)
add_subdirectory(game-server)
add_subdirectory(client)