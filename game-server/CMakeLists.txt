cmake_minimum_required(VERSION 3.16)
project(game-server)

include_directories(${CMAKE_SOURCE_DIR}/libs/networking/include)
include_directories(${CMAKE_SOURCE_DIR}/libs/persistence/include)
include_directories(${CMAKE_SOURCE_DIR}/libs/game/include)
file(GLOB_RECURSE SOURCES "*.cc")

add_executable(game-server ${SOURCES})

target_link_libraries(game-server PRIVATE networking-lib persistence-lib game-lib)