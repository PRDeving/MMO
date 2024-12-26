cmake_minimum_required(VERSION 3.16)
project(SQLite3 C)

add_library(sqlite3 SHARED sqlite3.c)
if(UNIX AND NOT APPLE)
    target_link_libraries(sqlite3 PRIVATE dl)
endif()
target_include_directories(sqlite3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})