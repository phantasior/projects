cmake_minimum_required(VERSION 3.12)

project(
    app
    VERSION 1.0
    DESCRIPTION "C++ SANDPILE PROJECT"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")
set(SRC_LIST app.cpp)

add_subdirectory(lib)
add_subdirectory(utils)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} PRIVATE parser)
target_link_libraries(${PROJECT_NAME} PRIVATE sandpile)
target_link_libraries(${PROJECT_NAME} PRIVATE bmp)
target_link_libraries(${PROJECT_NAME} PRIVATE utils)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
