cmake_minimum_required(VERSION 3.12)
project(
    app
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_FLAGS_INIT "--clean-first" "Wall" "-O2" "-g")
set(CMAKE_CXX_STANDARD 23)

add_subdirectory(lib)
add_subdirectory(bin)

enable_testing()
add_subdirectory(tests)