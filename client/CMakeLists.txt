cmake_minimum_required(VERSION 3.16)
project(client)

include_directories(${CMAKE_SOURCE_DIR}/external/json/include)
include_directories(${CMAKE_SOURCE_DIR}/libs/networking/include)
include_directories(${CMAKE_SOURCE_DIR}/libs/game/include)
file(GLOB_RECURSE SOURCES "*.cc")

add_executable(client ${SOURCES})
target_link_libraries(client PRIVATE networking-lib game-lib nlohmann_json)