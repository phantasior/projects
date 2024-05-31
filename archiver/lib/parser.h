#pragma once

#include "utils.h"

#include <iostream>
#include <vector>

const uint32_t ARCHIVE_NAME_MAX_SIZE = 256;

struct Option {
	bool del;
	bool list;
	bool append;
	bool create;
	bool concat;
	bool extract;

	uint32_t block_size;

	char archive_name[ARCHIVE_NAME_MAX_SIZE];

	std::vector<char*> files_list;

	Option();

	bool check();

	void print();
};

Option Parse(int argc, char** argv);