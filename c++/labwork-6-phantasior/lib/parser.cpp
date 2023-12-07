#include "parser.h"

#include <memory>
#include <iostream>

using namespace OMFL;

Object EMPTY_OBJECT;

Object::Object() : is_valid_(true) {}

Object::Object(std::variant<std::monostate, int, float, bool, std::string, std::vector<Object>> val)
    : val_(std::move(val)), is_valid_(true) {}

void SkipSpaces(size_t& ptr, const std::string& data) {
    while (ptr < data.size() && isspace(data[ptr])) {
        ptr++;
    }
}

bool SkipLine(size_t& ptr, const std::string& data) {
    bool was_comment = false;
    while (ptr < data.size() && data[ptr] != '\n') {
        if (data[ptr] == '#') {
            was_comment = true;
        }

        if (!isspace(data[ptr]) && !was_comment) {
            return false;
        }

        ptr++;
    }

    ptr++;

    return true;
}

Object ParseValue(size_t& ptr, const std::string& data) {
    std::variant<std::monostate, int, float, bool, std::string, std::vector<Object>> value;
    if (data[ptr] == '"') {
        std::string cur;
        ptr++;
        while (ptr < data.size() && data[ptr] != '"') {
            cur += data[ptr];
            ptr++;
        }

        if (ptr == data.size()) {
            return Object();
        }

        value = cur;
        ptr++;
        return Object(value);
    } else if (isdigit(data[ptr]) || data[ptr] == '+' || data[ptr] == '-') {
        std::string num;
        if (data[ptr] == '+' || data[ptr] == '-') {
            if (!isdigit(data[ptr + 1])) {
                return Object();
            }
            num += data[ptr];
            ptr++;
        }

        if (data[ptr] == '.') {
            return Object();
        }

        bool is_float = false;
        while (ptr < data.size() && isdigit(data[ptr])) {
            num += data[ptr];
            ptr++;
        }

        if (data[ptr] == '.') {
            num += data[ptr];
            is_float = true;
            ptr++;
        }

        if (is_float && !isdigit(data[ptr])) {
            return Object();
        }

        while (is_float && isdigit(data[ptr])) {
            num += data[ptr];
            ptr++;
        }


        if (is_float) {
            value = stof(num);
            return Object(value);
        }  else {
            value = stoi(num);
            return Object(value);
        }

    } else if (ptr + 3 < data.size() && data.substr(ptr, 4) == "true") {
        value = true;
        ptr += 4;
        return Object(value);
    } else if (ptr + 4 < data.size() && data.substr(ptr, 5) == "false") {
        value = false;
        ptr += 5;
        return Object(value);
    } else if (data[ptr] == '[') {
        std::vector<Object> cur;
        ptr++;

        if (data[ptr] == ']') {
            return Object();
        }

        while (data[ptr] != ']') {
            cur.push_back(ParseValue(ptr, data));

            if (data[ptr] == ']') {
                break;
            }

            if (data[ptr] != ',') {
                return Object();
            }

            ptr++;
            SkipSpaces(ptr, data);
        }

        ptr++;

        value = cur;
        return Object(value);
    }

    return Object();
}

Object OMFL::parse(const std::string& data) {
    Object global_section = Object();
    Object* cur_section = &global_section;

    size_t ptr = 0;
    while (ptr < data.size()) {
        SkipSpaces(ptr, data);
        if (ptr == data.size()) {
            break;
        }

        if (data[ptr] == '#') {
            SkipLine(ptr, data);
            continue;
        }
        
        if (data[ptr] == '[') { // SECTION
            cur_section = &global_section;

            ptr++;
            SkipSpaces(ptr, data);

            if (ptr == data.size() || data[ptr] == ']') {
                global_section.is_valid_ = false;
                return global_section;
            }

            size_t rptr = ptr; // ptr - beginning of sect_name, rptr - end
            while (ptr < data.size() && data[ptr] != ']') {
                while (rptr < data.size() && data[rptr] != '.' && data[rptr] != ']') {
                    if (!isalnum(data[rptr]) && data[rptr] != '-' && data[rptr] != '_') {
                        global_section.is_valid_ = false;
                        return global_section;
                    }

                    rptr++;
                }

                if (rptr == data.size()) {
                    global_section.is_valid_ = false;
                    return global_section;
                }

                std::string sect_name = data.substr(ptr, rptr - ptr);
                cur_section->sections_.insert( {sect_name, Object()} );
                cur_section = &cur_section->sections_.at(sect_name);

                if (data[rptr] == ']') { // now we handled section and now skip line
                    ptr = rptr + 1;
                    if (!SkipLine(ptr, data)) {
                        global_section.is_valid_ = false;
                        return global_section;
                    }

                    break;
                }

                rptr++;
                ptr = rptr;
            }
        } else {
            std::string key;
            while (ptr < data.size() && !isspace(data[ptr]) && data[ptr] != '=') {
                if (!isalnum(data[ptr]) && data[ptr] != '-' && data[ptr] != '_') {
                    global_section.is_valid_ = false;
                    return global_section;
                }

                key += data[ptr];
                ptr++;
            }
            
            SkipSpaces(ptr, data);
            if (data[ptr] != '=' || key == "") {
                global_section.is_valid_ = false;
                return global_section;
            }

            ptr++;
            while (data[ptr] == ' ') ptr++;
            if (data[ptr] == '\n') {
                global_section.is_valid_ = false;
                return global_section;
            }

            Object value = ParseValue(ptr, data);

            if (value.val_.index() == 0) { // monostate == error
                global_section.is_valid_ = false;
                return global_section;
            }

            if (!SkipLine(ptr, data)) {
                global_section.is_valid_ = false;
                return global_section;
            }
            
            if (cur_section->values_.find(key) != cur_section->values_.end()) {
                global_section.is_valid_ = false;
                return global_section;
            }

            cur_section->values_.insert({key, value});
        }
    }

    return global_section;
}

bool Object::IsInt() const {
    return val_.index() == 1;
}

bool Object::IsFloat() const {
    return val_.index() == 2;
}

bool Object::IsBool() const {
    return val_.index() == 3;
}

bool Object::IsString() const {
    return val_.index() == 4;
}

bool Object::IsArray() const {
    return val_.index() == 5;
}

bool Object::valid() const {
    return is_valid_;
}

int Object::AsInt() const {
    return std::get<int>(val_);
}

int Object::AsIntOrDefault(int def) const {
    if (IsInt()) {
        return std::get<int>(val_);
    } else {
        return def;
    }
}

float Object::AsFloat() const {
    return std::get<float>(val_);
}

float Object::AsFloatOrDefault(float def) const {
    if (IsFloat()) {
        return std::get<float>(val_);
    } else {
        return def;
    }
}

const std::string& Object::AsString() const {
    return std::get<std::string>(val_);
}

std::string Object::AsStringOrDefault(const std::string& def) const {
    if (IsString()) {
        return std::get<std::string>(val_);
    } else {
        return def;
    }
}

bool Object::AsBool() const {
    return std::get<bool>(val_);
}

const Object& Object::Get(const std::string& key) const {
    const Object* res = this;
    size_t l = 0;
    size_t r = 0;

    while (r < key.size()) {
        std::string cur;
        while (r < key.size() && key[r] != '.') {
            cur += key[r];
            r++;
        }
        r++;
        l = r;


        auto it = res->sections_.find(cur);
        if (it != res->sections_.end()) {
            res = &it->second;
            continue;
        }

        it = res->values_.find(cur);
        if (it != res->values_.end()) {
            res = &it->second;
        }

        return *res;
    }

    return *res;
}

const Object& Object::operator[](int idx) const {
    auto& cur = std::get<std::vector<Object>>(val_);
    if (idx >= cur.size()) {
        return EMPTY_OBJECT;
    }

    return cur[idx];
}