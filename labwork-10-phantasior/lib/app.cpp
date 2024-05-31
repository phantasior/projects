#include "app.hpp"

using namespace ftxui;
using json = nlohmann::json;
namespace fs = std::filesystem;

void Weather::Run() {
    ReadConfig();

    for (const auto& city : cities_) {
        ParseCityWeather(city);
    }

    std::thread app([&] {
        std::unique_lock<std::mutex> lk(mutex_);
        while (true) {
            Clear();
            PrintDay(days_to_show_, cities_[cur_city_id_]);
            cv_.wait_for(lk, std::chrono::seconds(reload_frequency_), [&]{ return terminate || reload; });
            if (terminate) break;
            reload = false;
        }
    });

    auto screen = ScreenInteractive::TerminalOutput();
    auto renderer = Renderer([] { return text(""); });
    auto component = CatchEvent(renderer, [&](Event event) {
        if (event == Event::Character('q')) {
            screen.ExitLoopClosure()();
        } else if (event == Event::Character('=')) { // to press + without shift
            days_to_show_ = std::min(6ul, days_to_show_ + 1);
            ReloadApp(app);
        } else if (event == Event::Character('-')) {
            days_to_show_ = std::max(1ul, days_to_show_ - 1);
            ReloadApp(app);
        } else if (event == Event::Character('n')) {
            cur_city_id_ = cur_city_id_ == cities_.size() - 1 ? 0 : cur_city_id_ + 1;
            ReloadApp(app);
        } else if (event == Event::Character('p')) {
            cur_city_id_ = cur_city_id_ == 0 ? cities_.size() - 1 : cur_city_id_ - 1;
            ReloadApp(app);
        }
        return false;
    });

    screen.Loop(component);
    StopApp(app);
}

void Weather::ReadConfig() {
    fs::path root = fs::current_path().parent_path().parent_path();
    std::ifstream config(root / "config.json");

    if (!config.is_open()) {
        throw std::invalid_argument("Coulnd't find such config");
    }

    json data_ = json::parse(config);

    reload_frequency_ = data_["reload_frequency"];
    days_to_show_     = data_["days_to_show"];
    cities_           = data_["countries"].get<std::vector<std::string>>();
}

void Weather::ParseCityWeather(const std::string& city) {
    Coordinates coordinates = ParseCityLocation(city);

    cpr::Response weather_response = cpr::Get(
        cpr::Url{"https://api.open-meteo.com/v1/forecast"},
        cpr::Parameters{
            {"latitude", std::to_string(coordinates.latitude)},
            {"longitude", std::to_string(coordinates.longitude)},
            {"timezone", "GMT"},
            {"hourly", "relativehumidity_2m,cloudcover,temperature_2m,windspeed_10m,weathercode"},
        }
    );

    if (weather_response.status_code != 200) {
        throw std::invalid_argument("Couldn't connect, error code: " + std::to_string(weather_response.status_code));
    }

    data_[city] = json::parse(weather_response.text);
}

Coordinates Weather::ParseCityLocation(const std::string& city) {
    std::string token = "nBkIqDSU2v9LFGPNyIIsow==VAVLcTski2uJ82hD";
    std::string city_request_url = "https://api.api-ninjas.com/v1/city";

    cpr::Response city_response = cpr::Get(
        cpr::Url{city_request_url},
        cpr::Header{{"X-Api-Key", token}},
        cpr::Parameters{{"name", city}}
    );    

    if (city_response.status_code != 200) {
        throw std::invalid_argument("Couldn't connect");
    }

    json city_info = json::parse(city_response.text);

    if (city_info.empty()) {
        throw std::invalid_argument("No such city");
    }

    city_info = city_info.get<std::vector<json>>()[0];

    return {
        city_info["latitude"].get<double>(),
        city_info["longitude"].get<double>()
    };
}

void Weather::PrintDay(size_t day, const std::string& city) const {
    size_t morning = GetHour(day, TimeOfDay::Morning);
    size_t noon = GetHour(day, TimeOfDay::Noon);
    size_t evening = GetHour(day, TimeOfDay::Evening);
    size_t night = GetHour(day, TimeOfDay::Night);

    Date date = ParseDate(city, morning);
    std::string formatted_date = GetFormattedDate(date);
    
    size_t total_wheater_length = 81;
    std::cout << "├";
    for(size_t i = 0; i < (total_wheater_length - city.size() - 2) / 2; ++i) std::cout << "─";
    std::cout << city;
    for(size_t i = 0; i < (total_wheater_length - city.size() - 2) / 2; ++i) std::cout << "─";
    std::cout << "┤\n";

    std::cout << "                                 ┌────────────┐                                   \n";
    std::cout << "┌───────────────────┬────────────┤ " << formatted_date <<"  ├─────────────┬───────────────────┐\n";
    std::cout << "│      Morning      │    Noon    └──────┬─────┘  Evening    │       Night       │\n";
    std::cout << "├───────────────────┼───────────────────┼───────────────────┼───────────────────┤\n";


    std::vector<std::vector<std::string>> day_data_;
    std::vector<size_t> day_stamps = {morning, noon, evening, night};
    for (size_t stamp : day_stamps) {
        std::string temp = std::to_string(data_.at(city)["hourly"]["temperature_2m"].get<std::vector<double>>()[stamp]).substr(0, 3) + "°C │";
        std::string wind_speed = std::to_string(data_.at(city)["hourly"]["windspeed_10m"].get<std::vector<int>>()[stamp]);
        wind_speed += (wind_speed.size() == 2 ? "km/h│" : "km/h │");

        std::string cloud_cover = "⛅️" + std::to_string(data_.at(city)["hourly"]["cloudcover"].get<std::vector<int>>()[stamp]);
        if (cloud_cover.size() == 9) {
            cloud_cover += "%│";
        } else if (cloud_cover.size() == 8) {
            cloud_cover += "% │";
        } else {
            cloud_cover += "%  │";
        } 

        day_data_.push_back({
            "      │",
            temp,
            cloud_cover,
            wind_speed,
            "      │"
        });
    }

    for (size_t row = 0; row < 5; ++row) {
        std::cout << "│";
        for (size_t i = 0; i < 4; ++i) {
            std::cout << WeatherTypeToImage(GetWeatherType(city, IntToTimeOfDay(i)), row) << day_data_[i][row];
        }

        std::cout << '\n';
    
    }

    std::cout << "└───────────────────┴───────────────────┴───────────────────┴───────────────────┘\n";
}

WeatherType Weather::GetWeatherType(const std::string& city, size_t hour) const {
    size_t code = data_.at(city)["hourly"]["weathercode"].get<std::vector<size_t>>()[hour];
    if (code < 4) {
        return WeatherType::Cloudy;
    } else if ((10 <= code &&  code <= 12) || code == 12 || code == 28 || (40 <= code && code <= 59)) {
        return WeatherType::Fog;
    } else if ((13 <= code && code <= 19) || code == 21 || code == 24 || code == 25 || code == 29 || (60 <= code && code <= 99)) {
        return WeatherType::Rain;
    } else if (code == 20 || code == 26 || code == 27) {
        return WeatherType::Snow; // 20 26 27
    } else if (code < 10 || (30 <= code && code <= 39)) {
        return WeatherType::Sunny;
    } else {
        return WeatherType::Unknown;
    }
}

Date Weather::ParseDate(const std::string& city, size_t hour) const {
    std::string date = data_.at(city)["hourly"]["time"].get<std::vector<std::string>>()[hour]; // "2023-04-01T00:00"

    size_t year = 1000 * (date[0] - '0') + 100 * (date[1] - '0') + 10 * (date[2] - '0') + (date[4] - '0');
    size_t month = 10 * (date[5] - '0') + (date[6] - '0');
    size_t day = 10 * (date[8] - '0') + (date[9] - '0');
    size_t day_of_week = GetDayOfWeek(year, month, day);

    return { year, month, day, day_of_week };
}

void Weather::Clear() noexcept {
    #if defined _WIN32
        system("cls");#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
        system("clear");    std::cout<< u8"\033[2J\033[1;1H";
    #elif defined (__APPLE__)    
        system("clear");
    #endif
}

void Weather::ReloadApp(std::thread& app) {
    {
        std::lock_guard<std::mutex> lk(mutex_);
        reload = true;
    }
    cv_.notify_all();
}

void Weather::StopApp(std::thread& app) {
    cv_.notify_all();
    {
        std::lock_guard<std::mutex> lk(mutex_);
        terminate = true;
    }
    cv_.notify_all();

    app.join();
}
