cmake_minimum_required(VERSION 3.0.0)
project(mp3 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_INIT "--clean-first")

link_directories(lib)
add_subdirectory(lib)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} parser)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})