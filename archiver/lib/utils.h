#pragma once

#include <cinttypes>
#include <string>

#ifdef __DEBUG__
	#define debug(x) (std::cout << #x << ": " << x << '\n')
#else
	#define debug(x)
#endif

const char temp_archive_name[] = "_TMP_ARCHIVE_NAME_";

void Error(const char* message);

uint64_t GetFileSize(const char* file_name);

std::vector<char*> Split(char* str, const char* sep);

void Usage();
