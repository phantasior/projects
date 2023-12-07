#pragma once

#include <filesystem>
#include <istream>
#include <map>
#include <vector>
#include <variant>

namespace OMFL {

class Object {
public:
    Object();
    Object(std::variant<std::monostate, int, float, bool, std::string, std::vector<Object>> val);

    const Object& Get(const std::string& key) const;

    int AsInt() const;
    int AsIntOrDefault(int def) const;
    float AsFloat() const;
    float AsFloatOrDefault(float def) const;
    const std::string& AsString() const;
    std::string AsStringOrDefault(const std::string& def) const;
    bool AsBool() const;

    bool IsInt() const;
    bool IsFloat() const;
    bool IsBool() const;
    bool IsString() const;
    bool IsArray() const;
    bool valid() const;

    const Object& operator[](int) const;

private:
    friend Object parse(const std::string& data);

    std::map<std::string, Object> values_;
    std::map<std::string, Object> sections_;
    std::variant<std::monostate, int, float, bool, std::string, std::vector<Object>> val_;
    bool is_valid_;
};

Object parse(const std::string& data);

} // namespace OMFL