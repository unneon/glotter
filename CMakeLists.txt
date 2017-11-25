cmake_minimum_required(VERSION 3.0)
project(glotter)

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES include/glotter.hpp src/glotter.cpp)
add_library(glotter ${SOURCE_FILES})

install(TARGETS glotter DESTINATION lib)
install(FILES include/glotter.hpp DESTINATION include)
install(DIRECTORY web DESTINATION share/glotter)

# Dependency - uWebSockets
find_path(UWEBSOCKETS_INCLUDE_DIR uWS/uWS.h)
find_library(UWEBSOCKETS_LIBRARY uWS)
include_directories(glotter ${UWEBSOCKETS_INCLUDE_DIR})
target_link_libraries(glotter ${UWEBSOCKETS_LIBRARY})
