#pragma once

#include <filesystem>
#include <istream>
#include <map>
#include <vector>
#include <variant>


namespace OMFL {

class Object {
public:
    enum Type {
        Section,
        Int,
        Float,
        String,
        Bool,
        Array,
        Error
    };

    Object();
    Object(Object::Type type);
    Object(Object::Type type, const std::variant<int, float, bool, std::string, std::vector<std::shared_ptr<Object>>>& val);

    Object& Get(const std::string& key) const;

    int         AsInt();
    int         AsIntOrDefault(int def);
    float       AsFloat();
    float       AsFloatOrDefault(float def);
    std::string AsString();
    std::string AsStringOrDefault(const std::string& def);
    bool        AsBool();

    bool IsInt();
    bool IsFloat();
    bool IsBool();
    bool IsString();
    bool IsArray();
    bool valid() const;

    Object& operator[](int);

    friend Object parse(const std::string& data);
private:
    std::map<std::string, std::shared_ptr<Object>> values_;
    std::map<std::string, std::shared_ptr<Object>> sections_;
    std::variant<int, float, bool, std::string, std::vector<std::shared_ptr<Object>>> val_;
    bool is_valid_;
    Type type_;
};


Object parse(const std::string& data);

} // namespace OMFL