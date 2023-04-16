#pragma once

#include "weather_formatter.hpp"

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/mouse.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

#include <iostream>
#include <chrono>
#include <thread>

struct Coordinates {
    double latitude;
    double longitude;
};

class Weather {
public:
    void Run();
    void ReadConfig();

    void PrintDay(size_t day, const std::string& city) const; // 0-indexed

    void StopApp(std::thread& app);
    void ReloadApp(std::thread& app);

    static void Clear() noexcept;
private:
    Date ParseDate(const std::string& city, size_t hour) const;
    WeatherType GetWeatherType(const std::string& city, size_t hour) const;

    Coordinates ParseCityLocation(const std::string& city);
    void ParseCityWeather(const std::string& city);

    size_t cur_city_id_ = 0;
    size_t reload_frequency_;
    size_t days_to_show_ = 1;

    std::vector<std::string> cities_;
    std::map<std::string, nlohmann::json> data_;

    std::mutex mutex_;
    std::condition_variable cv_;

    bool terminate = false;
    bool reload = false;
};