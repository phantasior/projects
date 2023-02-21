#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <climits>

namespace ArgumentParser {

enum ArgumentType {
    String,
    Int,
    Bool // flag
};

class Argument {
    public:
        Argument() {};
        ~Argument() {};
        Argument(char short_arg_);
        Argument(const std::string& arg_);
        Argument(char short_arg_, const std::string& arg_);
        Argument(const std::string& arg_, const std::string& desc_);
        Argument(char short_arg_, const std::string& arg_, const std::string& desc_);

        Argument& Default(const char* value_);
        Argument& Default(int value_);
        Argument& Default(bool value_);


        Argument& StoreValue(std::string& store_);
        Argument& StoreValue(int& store_);
        Argument& StoreValue(bool& store_);

        Argument& StoreValue(std::vector<std::string>& multi_store_str_);
        Argument& StoreValue(std::vector<int>& multi_store_int_);

        Argument& MultiValue(size_t min_args_count_ = 0);

        Argument& Positional();

        void Print() const;

        ArgumentType type;

        bool is_multi_store_added = false;
        bool is_store_added = false;
        bool is_positional = false;
        bool is_multi_value = false;
        bool is_default = false;
        size_t min_args_count = INT_MAX;

        //
        bool is_flag_enabled = false;
        bool* bool_store;


        char short_arg = '\0';
        std::string arg = "";
        std::string desc = "";

        std::string str_value = "";
        std::string* str_store;

        int int_value = -1;
        int* int_store;

        std::vector<int>* multi_store_int;
        std::vector<std::string>* multi_store_str;
};

} // namespace ArgumentParser