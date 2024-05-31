#include "Argument.h"

#include <iostream>

using namespace ArgumentParser;

Argument::Argument(char short_arg_) {
    short_arg = short_arg_;
}

Argument::Argument(const std::string& arg_) {
    arg = arg_;
}

Argument::Argument(char short_arg_, const std::string& arg_) {
    arg = arg_;
    short_arg = short_arg_;
}

Argument::Argument(const std::string& arg_, const std::string& desc_) {
    arg = arg_;
    desc = desc_;
}

Argument::Argument(char short_arg_, const std::string& arg_, const std::string& desc_) {
    short_arg = short_arg_;
    arg = arg_;
    desc = desc_;
}


Argument& Argument::Default(const char* value_) {
    this->is_default = true;
    this->str_value = std::string(value_);
    return *this;
}

Argument& Argument::Default(const int value_) {
    this->is_default = true;
    this->int_value = value_;
    return *this;
}

Argument& Argument::Default(const bool value_) {
    this->is_default = true;
    this->is_flag_enabled = value_;
    return *this;
}

Argument& Argument::StoreValue(std::string& store_) {
    is_store_added = true;
    str_store = &store_;
    return *this;
}

Argument& Argument::StoreValue(int& store_) {
    is_store_added = true;
    int_store = &store_;
    return *this;
}

Argument& Argument::StoreValue(bool& store_) {
    is_store_added = true;
    bool_store = &store_;
    return *this;
}

Argument& Argument::StoreValue(std::vector<int>& multi_store_int_) {
    this->is_multi_store_added = true;
    this->multi_store_int = &multi_store_int_;
    return *this;
}

Argument& Argument::StoreValue(std::vector<std::string>& multi_store_str_) {
    this->is_multi_store_added = true;
    this->multi_store_str = &multi_store_str_;
    return *this;
}

Argument& Argument::MultiValue(size_t min_args_count_) {
    this->is_multi_value = true;
    this->min_args_count = min_args_count_;
    return *this;
}

Argument& Argument::Positional() {
    this->is_positional = true;
    return *this;
}

void Argument::Print() const {
    if (short_arg) std::cout << "short argument: " << short_arg << '\n';
    if (!arg.empty()) std::cout << "argument: " << arg << '\n';
    if (!str_value.empty()) std::cout << "string value: " << str_value << '\n';
    if (int_value != -1) std::cout << "string value: " << int_value << '\n';
    if (!desc.empty()) std::cout << "description: " << desc << '\n';
    if (is_multi_store_added) {
        std::cout << "multi int store: ";
        for (auto& i : *multi_store_int) std::cout << i << ' ';
        std::cout << '\n';
    }
}
