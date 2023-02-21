#include "utils.h"

#include <iostream>
#include <fstream>
#include <vector>

void Error(const char* message) {
	std::cerr << message << '\n';
	exit(1);
}


uint64_t GetFileSize(const char* file_name) {
	std::ifstream file(file_name, std::ifstream::ate | std::ifstream::binary);

	if (!file.is_open()) {
        Error("Can't open this file :(");
    }

	return file.tellg();
}

std::vector<char*> Split(char* str, const char* sep) {
	std::vector<char*> ans;

	char * cur;

	cur = strtok(str, sep);
	while (cur) {
		ans.push_back(cur);
		cur = strtok(NULL, sep);
	}

	return ans;
}  

void Usage() {
	std::cout << "Archiver app:\n";
	std::cout << "-f, --file=[ARCHNAME]    Set archive name [necessary]\n";
	std::cout << "-c, --create             Create new archive\n";
	std::cout << "-l, --list               Print list of files in archive\n";
	std::cout << "-x, --extract            Extract files from archive\n";
	std::cout << "-a, --append             Add file into archive\n";
	std::cout << "-d, --delete             Delete file from archive\n";
	std::cout << "-A, --concatenate        Merge to archives\n";
}