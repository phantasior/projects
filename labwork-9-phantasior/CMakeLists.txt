cmake_minimum_required(VERSION 3.12)
project(
    aloc VERSION 0.1.0
    LANGUAGES CXX
    )


set(CMAKE_CXX_FLAGS_INIT "--clean-first" "Wall" "-O2" "-g" "-std=c++17" "-DCMAKE_BUILD_TYPE=Release")
set(CMAKE_CXX_STANDARD 20)

add_definitions(-w)

link_directories(lib)

add_subdirectory(lib)
add_subdirectory(bin)


# enable_testing()
# add_subdirectory(tests)