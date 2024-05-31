#pragma once

#include <array>
#include <stdexcept>
#include <map>
#include <string>
#include <vector>

enum WeatherType {
    Unknown, 
    Cloudy,
    Fog,
    Rain,
    Snow,
    Sunny
};

enum TimeOfDay {
    Morning,
    Noon,
    Evening,
    Night
};

struct Date {
    size_t year;
    size_t month;
    size_t day;
    size_t day_of_week;
};

TimeOfDay IntToTimeOfDay(size_t);

size_t GetHour(size_t day, TimeOfDay type);

size_t GetDayOfWeek(size_t year, size_t month, size_t day);

std::string& WeatherTypeToImage(WeatherType type, size_t row);

std::string& DayOfWeekToString(size_t);

std::string GetFormattedDate(const Date& date);