#include "utils.h"

#include <fstream>
#include <random>

void GenerateRandomInputFile(const Sandpile* sandpile) {
	std::ofstream out_file(sandpile->input_path);

	std::random_device dev;
    std::mt19937 rng(dev());

    std::uniform_int_distribution<std::mt19937::result_type> get_random_x(1, sandpile->width);
    std::uniform_int_distribution<std::mt19937::result_type> get_random_y(1, sandpile->length);
    std::uniform_int_distribution<std::mt19937::result_type> get_random_cnt(1, MAX_SANDPILE_AMOUNT);

    for (int i = 0; i < sandpile->width * sandpile->length / 2; ++i) {
    	out_file << std::to_string(get_random_x(rng)) << '\t' 
    			<< std::to_string(get_random_y(rng)) << '\t' 
    			<< std::to_string(get_random_cnt(rng)) << '\n';
    }

	out_file.close();
}

void GenerateCentralInputFile(const Sandpile* sandpile, const uint32_t count) {
	std::ofstream out_file(sandpile->input_path);

	out_file << std::to_string(sandpile->width / 2 + 1) << '\t' 
			<< std::to_string(sandpile->length / 2 + 1) << '\t' 
         	<< std::to_string(count) << '\n';


	out_file.close();
}