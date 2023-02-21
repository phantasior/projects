#pragma once

#include "Argument.h"

#include <iostream>
#include <string_view>
#include <string>
#include <vector>
#include <climits>

const std::string ERROR_MESSAGE = "NO SUCH VALUE";

namespace ArgumentParser {

class ArgParser {
    public:
        ArgParser(const std::string& project_name);
        ~ArgParser();

        bool Parse(const std::vector<std::string_view>& args);
        bool Parse(const std::vector<std::string>& args);
        bool Parse(int argc, char** argv);

        void ParseWithEqualSign(const std::vector<std::string_view>& args, size_t cur_arg_pos, size_t equal_sign_pos);
        void ParseWithoutEqualSign(const std::vector<std::string_view>& args, size_t& cur_arg_pos);
        void SetUpArgument(Argument* arg, const std::string_view& value);

        Argument& AddStringArgument(char short_arg);
        Argument& AddStringArgument(const std::string& arg);
        Argument& AddStringArgument(char short_arg, const std::string& arg);
        Argument& AddStringArgument(char short_arg, const std::string& arg, const std::string& desc);
        
        Argument& AddIntArgument(char short_arg);
        Argument& AddIntArgument(const std::string& arg);
        Argument& AddIntArgument(char short_arg, const std::string& arg);
        Argument& AddIntArgument(const std::string& arg, const std::string& desc);
        Argument& AddIntArgument(char short_arg, const std::string& arg, const std::string& desc);

        Argument& AddFlag(char short_arg);
        Argument& AddFlag(const std::string& arg);
        Argument& AddFlag(char short_arg, const std::string& arg);
        Argument& AddFlag(const std::string& arg, const std::string& desc);
        Argument& AddFlag(char short_arg, const std::string& arg, const std::string& desc);
        Argument& AddHelp(char short_arg, const std::string& arg, const std::string& desc);

        std::string HelpDescription() const;
        std::string GetStringValue(const std::string& arg) const;

        bool Help() const;
        bool GetFlag(const std::string& arg) const;
        bool CheckArgsForCorrectness() const;
        bool IsEquals(const Argument* arg, const std::string_view& arg_name) const;

        int GetIntValue(const std::string& arg) const ;
        int GetIntValue(const std::string& arg, size_t pos) const;

        void Print() const;

    private:
        bool is_help_enabled_ = false;
        Argument* Help_ = nullptr;

        std::string project_name_;
        std::vector<Argument*> arguments_;
};

} // namespace ArgumentParser
