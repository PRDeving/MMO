project(networking-lib)

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/external/enet/include)
file(GLOB_RECURSE SOURCES "*.cc")

add_library(networking-lib STATIC ${SOURCES})
target_include_directories(
    networking-lib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/external/enet/include>
)
target_link_libraries(networking-lib enet)