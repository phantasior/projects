add_library(weather app.cpp app.hpp weather_formatter.cpp weather_formatter.hpp)
target_link_libraries(
    weather
    PUBLIC cpr::cpr 
    PUBLIC nlohmann_json::nlohmann_json
    PUBLIC ftxui::screen
    PUBLIC ftxui::dom
    PUBLIC ftxui::component
)
target_include_directories(weather PUBLIC ${PROJECT_SOURCE_DIR})
