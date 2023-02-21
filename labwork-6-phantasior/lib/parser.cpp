#include "parser.h"

#include <memory>
#include <iostream>

using namespace OMFL;

Object EMPTY_OBJECT;

Object::Object() 
    : is_valid_(true)
    , type_(Object::Error)
{}

Object::Object(Object::Type type) 
    : is_valid_(true)
    , type_(type)
{}

Object::Object(Object::Type type, const std::variant<int, float, bool, std::string, std::vector<std::shared_ptr<Object>>>& val) 
    : type_(type)
    , val_(val)
{}

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

std::shared_ptr<Object> ParseValue(size_t& ptr, const std::string& data) {
    std::variant<int, float, bool, std::string, std::vector<std::shared_ptr<Object>>> value;
    if (data[ptr] == '"') {
        std::string cur;
        ptr++;
        while (ptr < data.size() && data[ptr] != '"') {
            cur += data[ptr];
            ptr++;
        }

        if (ptr == data.size()) {
            return std::shared_ptr<Object>(new Object());
        }

        value = cur;
        ptr++;

        return std::shared_ptr<Object>(new Object(Object::Type::String, value));
    } else if (isdigit(data[ptr]) || data[ptr] == '+' || data[ptr] == '-') {
        std::string num;
        if (data[ptr] == '+' || data[ptr] == '-') {
            if (!isdigit(data[ptr + 1])) {
                return std::shared_ptr<Object>(new Object());
            }
            num += data[ptr];
            ptr++;
        }

        if (data[ptr] == '.') {
            return std::shared_ptr<Object>(new Object());
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
            return std::shared_ptr<Object>(new Object());
        }

        while (is_float && isdigit(data[ptr])) {
            num += data[ptr];
            ptr++;
        }


        if (is_float) {
            value = stof(num);
            return std::shared_ptr<Object>(new Object(Object::Type::Float, value));
        }  else {
            value = stoi(num);
            return std::shared_ptr<Object>(new Object(Object::Type::Int, value));
        }

    } else if (ptr + 3 < data.size() && data.substr(ptr, 4) == "true") {
        value = true;
        ptr += 4;
        return std::shared_ptr<Object>(new Object(Object::Type::Bool, value));
    } else if (ptr + 4 < data.size() && data.substr(ptr, 5) == "false") {
        value = false;
        ptr += 5;
        return std::shared_ptr<Object>(new Object(Object::Type::Bool, value));
    } else if (data[ptr] == '[') {
        std::vector<std::shared_ptr<Object>> cur;
        ptr++;

        if (data[ptr] == ']') {
            return std::shared_ptr<Object>(new Object());
        }

        while (data[ptr] != ']') {
            cur.push_back(ParseValue(ptr, data));

            if (data[ptr] == ']') {
                break;
            }

            if (data[ptr] != ',') {
                return std::shared_ptr<Object>(new Object());
            }

            ptr++;
            SkipSpaces(ptr, data);
        }

        ptr++;

        value = cur;
        return std::shared_ptr<Object>(new Object(Object::Type::Array, value));
    }

    return std::shared_ptr<Object>(new Object());
}

Object OMFL::parse(const std::string& data) {
    std::shared_ptr<Object> global_section(new Object());
    std::shared_ptr<Object> cur_section = global_section;

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
        
        if (data[ptr] == '[') {
            cur_section = std::shared_ptr<Object>(global_section);

            ptr++;
            SkipSpaces(ptr, data);

            if (ptr == data.size() || data[ptr] == ']') {
                global_section->is_valid_ = false;
                return *global_section;
            }

            size_t rptr = ptr; // ptr - beginning of sect_name, rptr - end
            while (ptr < data.size() && data[ptr] != ']') {
                while (rptr < data.size() && data[rptr] != '.' && data[rptr] != ']') {
                    if (!isalnum(data[rptr]) && data[rptr] != '-' && data[rptr] != '_') {
                        global_section->is_valid_ = false;
                        return *global_section;
                    }

                    rptr++;
                }

                if (rptr == data.size()) {
                    global_section->is_valid_ = false;
                    return *global_section;
                }

                std::string sect_name = data.substr(ptr, rptr - ptr);
                if (cur_section->sections_.find(sect_name) == cur_section->sections_.end()) {
                    cur_section->sections_.insert({sect_name, std::shared_ptr<Object>(new Object(Object::Type::Section))});
                }

                cur_section = cur_section->sections_.at(sect_name);

                if (data[rptr] == ']') { // now we handled section and now skip line
                    ptr = rptr + 1;
                    if (!SkipLine(ptr, data)) {
                        global_section->is_valid_ = false;
                        return *global_section;
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
                    global_section->is_valid_ = false;
                    return *global_section;
                }

                key += data[ptr];
                ptr++;
            }


            SkipSpaces(ptr, data);
            if (data[ptr] != '=' || key == "") {
                global_section->is_valid_ = false;
                return *global_section;
            }

            ptr++;
            while (data[ptr] == ' ') ptr++;
            if (data[ptr] == '\n') {
                global_section->is_valid_ = false;
                return *global_section;
            }

            std::shared_ptr<Object> value = ParseValue(ptr, data);

            if (value->type_ == Object::Type::Error) {
                global_section->is_valid_ = false;
                return *global_section;
            }

            if (!SkipLine(ptr, data)) {
                global_section->is_valid_ = false;
                return *global_section;
            }
            
            if (cur_section->values_.find(key) != cur_section->values_.end()) {
                global_section->is_valid_ = false;
                return *global_section;
            }

            cur_section->values_.insert({key, value});
        }
    }

    return *global_section;
}

bool Object::IsInt() {
    return type_ == Type::Int;
}

bool Object::IsFloat() {
    return type_ == Type::Float;
}

bool Object::IsBool() {
    return type_ == Type::Bool;
}

bool Object::IsString() {
    return type_ == Type::String;
}

bool Object::IsArray() {
    return type_ == Type::Array;
}

bool Object::valid() const {
    return is_valid_;
}

int Object::AsInt() {
    return std::get<int>(val_);
}

int Object::AsIntOrDefault(int def) {
    if (IsInt()) {
        return std::get<int>(val_);
    } else {
        return def;
    }
}

float Object::AsFloat() {
    return std::get<float>(val_);
}

float Object::AsFloatOrDefault(float def) {
    if (IsFloat()) {
        return std::get<float>(val_);
    } else {
        return def;
    }
}

std::string Object::AsString() {
    return std::get<std::string>(val_);
}

std::string Object::AsStringOrDefault(const std::string& def) {
    if (IsString()) {
        return std::get<std::string>(val_);
    } else {
        return def;
    }
}

bool Object::AsBool() {
    return std::get<bool>(val_);
}

Object& Object::Get(const std::string& key) const {
    std::shared_ptr<Object> res = std::make_shared<Object>(*this);
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

        if (res->sections_.find(cur) != res->sections_.end()) {
            res = res->sections_.at(cur);
        } else if (res->values_.find(cur) != res->values_.end()) {
            res = res->values_.at(cur);
        } else {
            return *res;
        }
    }

    return *res;
}

Object& Object::operator[](int idx) {
    auto cur = std::get<std::vector<std::shared_ptr<Object>>>(val_);
    if (idx >= cur.size()) {
        return EMPTY_OBJECT;
    }

    return *cur[idx]; 
}