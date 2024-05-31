#include "parser.h"
#include "utils.h"
 
#include <iostream>

Option Parse(int argc, char** argv) {
	Option option;
	size_t ptr = 1;

	if (argc < 2) {
		Usage();
		exit(1);
	}

	while (ptr < argc) {
		char* arg = argv[ptr];

		if (strncmp(arg, "--file", 6) == 0) {
			if (strchr(arg, '=') != NULL) {
				strcpy(option.archive_name, Split(arg, "=")[1]);
			}

			ptr++;
		} else if (strcmp(arg, "-f") == 0) {
			strcpy(option.archive_name, argv[ptr + 1]);
			ptr += 2;
		} else if (strcmp(arg, "-x") == 0 || strcmp(arg, "--extract") == 0) {
			option.extract = true;
			ptr++;
		} else if (strcmp(arg, "-c") == 0 || strcmp(arg, "--create") == 0) {
			option.create = true;
			ptr++;
		} else if (strcmp(arg, "-a") == 0 || strcmp(arg, "--append") == 0) {
			option.append = true;
			ptr++;
		} else if (strcmp(arg, "-d") == 0 || strcmp(arg, "--delete") == 0) {
			option.del = true;
			ptr++;
		} else if (strcmp(arg, "-A") == 0 || strcmp(arg, "--concatenate") == 0) {
			option.concat = true;
			ptr++;
		} else if (strcmp(arg, "-l") == 0 || strcmp(arg, "--list") == 0) {
			option.list = true;
			ptr++;
		} else if (strcmp(arg, "-b") == 0 || strcmp(arg, "--block-size") == 0) {
			option.block_size = atoi(argv[ptr + 1]);
			ptr += 2;
		} else {
			option.files_list.push_back(arg);
			ptr++;	
		}
	}

	if (!option.check()) {
		Error("Incorrect arguments!");
	}

	return option;
}

bool Option::check() {
	if (del + list + create + append + concat + extract != 1) {
		return false;
	}

	if (strcmp(archive_name, "") == 0) {
		return false;
	}

	if (create && block_size == 0) {
		return false;
	}

	return true;
}

void Option::print() {
	std::cout << "Files:\n";
	for (auto i : files_list) std::cout << i << ' ';
	std::cout << "\nArchive name: " << archive_name << '\n';
}

Option::Option() {
	del = false;
	list = false;
	append = false;
	create = false;
	concat = false;
	extract = false;

	block_size = 0;
}