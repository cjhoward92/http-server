cmake_minimum_required(VERSION 3.13)

project(http)

include_directories(libuv/include)
add_subdirectory(libuv)

add_executable(http src/main.cpp)

target_link_libraries(http uv)