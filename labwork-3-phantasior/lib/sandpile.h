#pragma once

#include <deque>
#include <queue>
#include <cinttypes>
#include <iostream>

struct Sandpile {
	uint16_t length;
	uint16_t width;

	uint8_t offset_x = 0;
	uint8_t offset_y = 0;

	std::deque<std::deque<uint64_t>> grid;
	std::queue<std::pair<size_t, size_t>> topple_queue;

	uint64_t max_iterations;
	uint64_t frequency;

	char* input_path;
	char* output_path;

	Sandpile() {
		length = 1;
		width = 1;

		max_iterations = INT_MAX;
		frequency = 0;
	}

	void Resize();

	void GetDataFromTSV();

	bool Topple();

	void print() const {
		for (int i = 0; i < length; ++i) {
			for (int j = 0; j < width; ++j) {
				std::cout << grid[i][j] << ' ';
			}

			std::cout << '\n';
		}

		std::cout << '\n';
	}
};
