cmake_minimum_required(VERSION 3.13)

project(http CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")

add_subdirectory(spdlog)
add_subdirectory(libuv)

add_executable(http src/main.cpp src/server.cpp)

target_link_libraries(http PRIVATE spdlog::spdlog)
target_link_libraries(http PRIVATE uv)