project(game-lib)

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/external/entt/src)
file(GLOB_RECURSE SOURCES "*.cc")

add_library(game-lib STATIC ${SOURCES})
target_include_directories(
    game-lib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/external/entt/src>
)
target_link_libraries(game-lib)