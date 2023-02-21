#include "ArgParser.h"
#include "utils.h"

#include <iostream>

using namespace ArgumentParser;

ArgParser::ArgParser(const std::string& project_name) {
    project_name_ = project_name;
}

ArgParser::~ArgParser() {
    if (Help_ != nullptr) {
        delete Help_;
    }

    for (Argument* argument : arguments_) {
        delete argument;
    }
}

bool ArgParser::Parse(int argc, char** argv) {
    std::vector<std::string_view> args;
    for (int i = 0; i < argc; ++i) {
        args.push_back(std::string_view(argv[i]));
    }

    return Parse(args);
}

bool ArgParser::Parse(const std::vector<std::string>& args) {
    std::vector<std::string_view> new_args;
    for (int i = 0; i < args.size(); ++i) {
        new_args.push_back(std::string_view(args[i]));
    }

    return Parse(new_args);
}

bool ArgParser::Parse(const std::vector<std::string_view>& args) {
    for (size_t i = 1; i < args.size(); ++i) {
        size_t equal_sign_pos = args[i].find("=");
        
        if (equal_sign_pos != std::string::npos) {
            ParseWithEqualSign(args, i, equal_sign_pos);
        } else {
            ParseWithoutEqualSign(args, i);
        }
    }
    
    return CheckArgsForCorrectness();
}

void ArgParser::ParseWithEqualSign(const std::vector<std::string_view>& args, size_t cur_arg_pos, size_t equal_sign_pos) {
    auto [arg_name, arg_value] = Split(args[cur_arg_pos], equal_sign_pos);

    for (auto& added_arg : arguments_) {
        if (!IsEquals(added_arg, arg_name)) {
            continue;
        }
        SetUpArgument(added_arg, arg_value);

        break;
    }
}

void ArgParser::ParseWithoutEqualSign(const std::vector<std::string_view>& args, size_t& cur_arg_pos) {
    std::string_view arg_name = GetArgName(args[cur_arg_pos]);
    size_t count_dashes = CountDashes(args[cur_arg_pos]);

    if (count_dashes == 1 || count_dashes == 2) {
        // Check for help flag

        if (IsEquals(Help_, arg_name)) {
            is_help_enabled_ = true;
            return;
        }

        // No equal sign but 1 or 2 dashes and not a flag = args with type --arg value
        for (Argument* added_arg : arguments_) {
            if (!IsEquals(added_arg, arg_name) || added_arg->type == ArgumentType::Bool) {
                continue;
            }

            if (cur_arg_pos >= args.size()) {
                throw "There is no value for this flag";
            }

            SetUpArgument(added_arg, args[++cur_arg_pos]);

            return;
        }


        // handle these flags -fml [one char flags]
        for (const char& flag : arg_name) {
            for (auto& added_args : arguments_) {

                if (added_args->type != ArgumentType::Bool || (added_args->arg != arg_name && added_args->short_arg != flag)) {
                    continue;
                }

                added_args->is_flag_enabled = true;
                if (added_args->is_store_added) {
                    *added_args->bool_store = true;
                }

                return;
            }
        }

        return;
    }

    if (count_dashes != 0) return;
    
    // below is no dashes and no equal sign - optional parameters
    for (auto& added_arg : arguments_) {
        if (added_arg->is_positional) {
            SetUpArgument(added_arg, arg_name);
            break;
        }
    }
}

void ArgParser::SetUpArgument(Argument* arg, const std::string_view& value) {
    switch(arg->type) {
    case ArgumentType::String:
        arg->str_value = value;

        if (arg->is_store_added) {
            *arg->str_store = value;
        }

        if (arg->is_multi_store_added) {
            (*arg).multi_store_str->push_back(value.data());
        }

        break;
    case ArgumentType::Int:
        arg->int_value = atoi(value.data());

        if (arg->is_store_added) {
            *arg->int_store = atoi(value.data());
        }

        if (arg->is_multi_store_added) {
            (*arg).multi_store_int->push_back(atoi(value.data()));
        }

        break;
    case ArgumentType::Bool:
        break;
    }
}

bool ArgParser::CheckArgsForCorrectness() const {
    if (is_help_enabled_) {
        return true;
    }

    for (auto& arg: arguments_) {
        switch(arg->type) {
        case ArgumentType::Int:
            if (arg->is_positional == false && arg->int_value == -1) {
                return false;
            } else if (arg->min_args_count != INT_MAX && arg->multi_store_int->size() < arg->min_args_count) {
                return false;
            }

            break;
        case ArgumentType::String:
            if (arg->multi_store_str && arg->min_args_count != INT_MAX && arg->multi_store_str->size() < arg->min_args_count) {
                return false;
            } else if (arg->is_positional == false && arg->str_value.empty()) {
                return false;
            }
            
            break;
        case ArgumentType::Bool:
            break;
        default:
            return false;
        }
    }

    return true;
}

Argument& ArgParser::AddStringArgument(char short_arg) {
    Argument* argument = new Argument(short_arg);
    argument->type = ArgumentType::String;
    arguments_.push_back(argument);
    return *argument;
}

Argument& ArgParser::AddStringArgument(const std::string& arg) {
    Argument* argument = new Argument(arg);
    argument->type = ArgumentType::String;
    arguments_.push_back(argument);
    return *argument;
}

Argument& ArgParser::AddStringArgument(char short_arg, const std::string& arg) {
    Argument* argument = new Argument(short_arg, arg);
    argument->type = ArgumentType::String;
    arguments_.push_back(argument);
    return *argument;
}

Argument& ArgParser::AddStringArgument(char short_arg, const std::string& arg, const std::string& desc) {
    Argument* argument = new Argument(short_arg, arg, desc);
    argument->type = ArgumentType::String;
    arguments_.push_back(argument);
    return *argument;
}

Argument& ArgParser::AddIntArgument(char short_arg) {
    Argument* argument = new Argument(short_arg);
    argument->type = ArgumentType::Int;
    arguments_.push_back(argument);
    return *argument;
}

Argument& ArgParser::AddIntArgument(const std::string& arg) {
    Argument* argument = new Argument(arg);
    argument->type = ArgumentType::Int;
    arguments_.push_back(argument);
    return *argument;
}

Argument& ArgParser::AddIntArgument(char short_arg, const std::string& arg) {
    Argument* argument = new Argument(short_arg, arg);
    argument->type = ArgumentType::Int;
    arguments_.push_back(argument);
    return *argument;
}

Argument& ArgParser::AddIntArgument(const std::string& arg, const std::string& desc) {
    Argument* argument = new Argument(arg, desc);
    argument->type = ArgumentType::Int;
    arguments_.push_back(argument);
    return *argument;
}

Argument& ArgParser::AddIntArgument(char short_arg, const std::string& arg, const std::string& desc) {
    Argument* argument = new Argument(short_arg, arg, desc);
    argument->type = ArgumentType::Int;
    arguments_.push_back(argument);
    return *argument;
}

Argument& ArgParser::AddFlag(char short_arg) {
    Argument* argument = new Argument(short_arg);
    argument->type = ArgumentType::Bool;
    arguments_.push_back(argument);
    return *argument;
}

Argument& ArgParser::AddFlag(const std::string& arg) {
    Argument* argument = new Argument(arg);
    argument->type = ArgumentType::Bool;
    arguments_.push_back(argument);
    return *argument;
}

Argument& ArgParser::AddFlag(char short_arg, const std::string& arg) {
    Argument* argument = new Argument(short_arg, arg);
    argument->type = ArgumentType::Bool;
    arguments_.push_back(argument);
    return *argument;
}

Argument& ArgParser::AddFlag(const std::string& arg, const std::string& desc) {
    Argument* argument = new Argument(arg, desc);
    argument->type = ArgumentType::Bool;
    arguments_.push_back(argument);
    return *argument;
}

Argument& ArgParser::AddFlag(char short_arg, const std::string& arg, const std::string& desc) {
    Argument* argument = new Argument(short_arg, arg, desc);
    argument->type = ArgumentType::Bool;
    arguments_.push_back(argument);
    return *argument;
}

std::string ArgParser::GetStringValue(const std::string& arg) const {
    for (Argument* argument: arguments_) {
        if (IsEquals(argument, arg)) {
            return argument->str_value;
        }
    }

    throw ERROR_MESSAGE;
}

int ArgParser::GetIntValue(const std::string& arg) const {
    for (Argument* argument: arguments_) {
        if (IsEquals(argument, arg)) {
            return argument->int_value;
        }
    }

    throw ERROR_MESSAGE;
}

int ArgParser::GetIntValue(const std::string& arg, size_t pos) const {
    for (Argument* argument: arguments_) {
        if (IsEquals(argument, arg)) {
            return (*argument->multi_store_int)[pos];
        }
    }

    throw ERROR_MESSAGE;
}

bool ArgParser::GetFlag(const std::string& arg) const {
    for (Argument* argument: arguments_) {
        if (IsEquals(argument, arg)) {
            return argument->is_flag_enabled;
        }
    }

    throw ERROR_MESSAGE;
}

Argument& ArgParser::AddHelp(char short_arg, const std::string& arg, const std::string& desc) {
    Help_ = new Argument(short_arg, arg, desc);
    return *Help_;
}

bool ArgParser::Help() const {
    return is_help_enabled_;
}

std::string ArgParser::HelpDescription() const {
    std::string desc;
    desc += project_name_ + "\n";
    for (Argument* arg : arguments_) {
        if (arg->short_arg != '\0') {
            desc += "-" + std::string(1, arg->short_arg) + ",  ";
        } else {
            desc += "    ";
        }

        if (arg->arg != "") {
            desc += "--" + arg->arg + ",  ";
        }

        if (arg->desc != "") {
            desc += arg->desc + " ";
        }

        if (arg->is_default) {
            desc += "[default = true] ";
        }

        if (arg->min_args_count != INT_MAX) {
            desc += "[repeated, min args = " + std::to_string(arg->min_args_count) + "] ";
        }

        desc += "\n";
    }

    desc += "\n -h, --help Display this help and exit\n";

    return desc;
}

bool ArgParser::IsEquals(const Argument* arg, const std::string_view& arg_name) const {
    return arg && (arg->arg == arg_name || (arg_name.size() == 1 && arg->short_arg == arg_name[0]));
}

void ArgParser::Print() const {
    for (const auto& i : arguments_) {
        i->Print();
        std::cout << '\n';
    }
}