#include "weather_formatter.hpp"

TimeOfDay IntToTimeOfDay(size_t id) {
    switch (id) {
        case 0:
            return TimeOfDay::Morning;
        case 1:
            return TimeOfDay::Noon;
        case 2:
            return TimeOfDay::Evening;
        case 3:
            return TimeOfDay::Night;
    }

    return TimeOfDay::Morning;
}

size_t GetHour(size_t day, TimeOfDay type) {
    switch (type) {
        case TimeOfDay::Morning:
            return day * 24 + 8; 
        case TimeOfDay::Noon:
            return day * 24 + 12; 
        case TimeOfDay::Evening:
            return day * 24 + 18; 
        case TimeOfDay::Night:
            return day * 24 + 22; 
    }

    throw std::runtime_error("unexpected TypeOfDay....");
}

size_t GetDayOfWeek(size_t year, size_t month, size_t day) {
    static std::array<int, 12> day_trick = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    year -= month < 3;  
    return ( year + year / 4 - year / 100 + year / 400 + day_trick[month - 1] + day) % 7;
}

std::string& WeatherTypeToImage(WeatherType type, size_t row) {
    static std::map<WeatherType, std::vector<std::string>> map_to_image = {
        {
            WeatherType::Unknown,
            {
                "    .-.      ",
                "     __)     ",
                "    (        ",
                "     `-᾿     ",
                "      •      " 
            }
        },
        {
            WeatherType::Cloudy,
            {
                "             ",
                "\033[38;5;250m     .--.    \033[0m",
                "\033[38;5;250m  .-(    ).  \033[0m",
                "\033[38;5;250m (___.__)__) \033[0m",
                "             ",
            }
        },
        {
            WeatherType::Fog,
            {
                "             ",
                "\033[38;5;251m _ - _ - _ - \033[0m",
                "\033[38;5;251m  _ - _ - _  \033[0m",
                "\033[38;5;251m _ - _ - _ - \033[0m",
                "             ",
            }
        },
        {
            WeatherType::Rain,
            {
                "\033[38;5;240;1m     .-.     \033[0m",
                "\033[38;5;240;1m    (   ).   \033[0m",
                "\033[38;5;240;1m   (___(__)  \033[0m",
                "\033[38;5;21;1m  ‚ʻ‚ʻ‚ʻ‚ʻ   \033[0m",
                "\033[38;5;21;1m  ‚ʻ‚ʻ‚ʻ‚ʻ   \033[0m",
            }
        },
        {
            WeatherType::Snow,
            {
                "\033[38;5;240;1m     .-.     \033[0m",
                "\033[38;5;240;1m    (   ).   \033[0m",
                "\033[38;5;240;1m   (___(__)  \033[0m",
                "\033[38;5;255;1m   * * * *   \033[0m",
                "\033[38;5;255;1m  * * * *    \033[0m",
            }
        },
        {
            WeatherType::Sunny,
            {
                "\033[38;5;226m    \\   /    \033[0m",
                "\033[38;5;226m     .-.     \033[0m",
                "\033[38;5;226m  ‒ (   ) ‒  \033[0m",
                "\033[38;5;226m     `-᾿     \033[0m",
                "\033[38;5;226m    /   \\    \033[0m",
            }
        }
    };

    return map_to_image[type][row];
}

std::string& DayOfWeekToString(size_t id) {
    static std::map<size_t, std::string> day_to_string = {
        {0, "Sun"},
        {1, "Mon"},
        {2, "Tue"},
        {3, "Whe"},
        {4, "Thu"},
        {5, "Fri"},
        {6, "Sat"},
    };

    return day_to_string[id];
}

std::string GetFormattedDate(const Date& date) {
    std::string res = DayOfWeekToString(date.day_of_week);
    res += ' ';

    std::string placeholder = std::to_string(date.day);
    if (placeholder.size() == 1) {
        placeholder.push_back('0');
        std::swap(placeholder[0], placeholder[1]);
    }

    res += placeholder;
    res += '.';

    placeholder.clear();
    placeholder = std::to_string(date.month);
    if (placeholder.size() == 1) {
        placeholder.push_back('0');
        std::swap(placeholder[0], placeholder[1]);
    }

    res += placeholder;
    return res;
}