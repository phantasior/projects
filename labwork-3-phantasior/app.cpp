#include "lib/parser.h"
#include "lib/sandpile.h"
#include "lib/bmp.h"
#include "utils/utils.h"

#include <iostream>
#include <iomanip>
#include <time.h>

int main(int argc, char** argv) {
	Sandpile* sandpile = Parse(argc, argv);

	GenerateCentralInputFile(sandpile, 100000);

	sandpile->GetDataFromTSV();

	uint64_t cur_iter = 0;
	clock_t start_time = clock();

	std::cout << "Program started!\n";

	while (sandpile->Topple()) {
		if (sandpile->frequency > 0 && cur_iter % sandpile->frequency == 0) {
			CreateSandpileBMP(sandpile, cur_iter);
		}

		if (cur_iter == sandpile->max_iterations) {
			break;
		}

		cur_iter++;
	}

	if (sandpile->frequency == 0) {
		CreateSandpileBMP(sandpile, 1);
	}

	if (cur_iter != sandpile->max_iterations) {
		std::cout << "Your sandpile is stabled!\n";
	} else {
		std::cout << "Maximum iterations done!\n";
		std::cout << "Sandpile is not stabled :(\n";
	}

	std::cout << "----------\n" <<
				 "Total Width: " << sandpile->width << '\n' <<
				 "Total Height: " << sandpile->length << '\n' <<
				 "Total Iteraions: " << cur_iter << '\n' <<
				 "Total Duration: " << std::fixed << std::setprecision(1) <<
				  	(static_cast<double>(clock() - start_time) / CLOCKS_PER_SEC) << '\n';
}
