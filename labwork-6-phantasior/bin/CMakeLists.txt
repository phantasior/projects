add_executable(app main.cpp)

target_link_libraries(app parser)
target_include_directories(app PRIVATE ${PROJECT_SOURCE_DIR})
