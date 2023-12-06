#pragma once

#include <vector>
#include <string>
#include <string_view>

size_t CountDashes(const std::string_view& str);

std::pair<std::string_view, std::string_view> Split(const std::string_view& str, size_t pos);

std::string_view GetArgName(const std::string_view& str);
