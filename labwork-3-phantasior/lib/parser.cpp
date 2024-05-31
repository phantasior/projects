#include "parser.h"

#include <iostream>

Sandpile* Parse(int argc, char** argv) {
	Sandpile* sandpile = new Sandpile();

	size_t ptr = 1;
	while (ptr < argc) {
		const char* arg = argv[ptr];
		
		if (strcmp(arg, "-l") == 0 || strcmp(arg, "--length") == 0) {
			sandpile->length = atoi(argv[ptr + 1]);
			ptr += 2;
		} else if (strcmp(arg, "-w") == 0 || strcmp(arg, "--width") == 0) {
			sandpile->width = atoi(argv[ptr + 1]);
			ptr += 2;
		} else if (strcmp(arg, "-i") == 0 || strcmp(arg, "--input") == 0) {
			sandpile->input_path = argv[ptr + 1];
			ptr += 2;
		} else if (strcmp(arg, "-o") == 0 || strcmp(arg, "--output") == 0) {
			sandpile->output_path = argv[ptr + 1];
			ptr += 2;
		} else if (strcmp(arg, "-m") == 0 || strcmp(arg, "--max-iter") == 0) {
			sandpile->max_iterations = atoi(argv[ptr + 1]);
			ptr += 2;
		} else if (strcmp(arg, "-f") == 0 || strcmp(arg, "--freq") == 0) {
			sandpile->frequency = atoi(argv[ptr + 1]);
			ptr += 2;
		} else {
			std::cout << "Unknown command\n";
			ptr += 1;
		}
	}

	sandpile->grid.resize(sandpile->length, std::deque<uint64_t>(sandpile->width, 0));

	return sandpile;
}
