#include "utils.h"

#include <cstring>
#include <cstdio>
#include <climits>

size_t CountDashes(const std::string_view& str) {
	size_t dashes_cnt = 0;
	while (dashes_cnt < str.size() && str[dashes_cnt] == '-') {
		dashes_cnt++;
	}

	return dashes_cnt;
}

std::string_view GetArgName(const std::string_view& str) {
	return str.substr(CountDashes(str));
}

std::pair<std::string_view, std::string_view> Split(const std::string_view& str, size_t pos) {
	size_t dashes_cnt = CountDashes(str);
	std::string_view arg = str.substr(dashes_cnt, pos - dashes_cnt);
	std::string_view val = str.substr(pos + 1, str.size() - pos - 1);
	return {arg, val};
}  
