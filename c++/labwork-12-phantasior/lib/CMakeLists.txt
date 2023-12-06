add_library(MySQL MySQL.cpp MySQL.h Parser/CharSource.cpp Parser/CharSource.h Parser/SQLParser.cpp Parser/SQLParser.h)
#target_link_libraries(MySQL parser)

#target_include_directories(MySQL PUBLIC ${PROJECT_SOURCE_DIR})