cmake_minimum_required(VERSION 3.13)

project(http CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")

set(HTTP_SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(HTTP_PARSER_DIR ${CMAKE_SOURCE_DIR}/http-parser)
set(HTTP_PARSER ${HTTP_PARSER_DIR}/http_parser.o)

add_custom_command(
  OUTPUT ${HTTP_PARSER}
  COMMAND make
  WORKING_DIRECTORY ${HTTP_PARSER_DIR}
)

include_directories(${HTTP_PARSER_DIR} ${HTTP_SRC_DIR})

add_subdirectory(spdlog)
add_subdirectory(libuv)

add_executable(http src/main.cpp src/server.cpp ${HTTP_PARSER})

set_source_files_properties(
  ${HTTP_PARSER}
  PROPERTIES
  EXTERNAL_OBJECT true
  GENERATED true
)

target_link_libraries(http PRIVATE spdlog::spdlog)
target_link_libraries(http PRIVATE uv)