project(persistence-lib)

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/external/hiredis)
file(GLOB_RECURSE SOURCES "*.cc")

add_library(persistence-lib STATIC ${SOURCES})

target_include_directories(
    networking-lib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/external/hiredis>
)
target_link_libraries(persistence-lib hiredis)