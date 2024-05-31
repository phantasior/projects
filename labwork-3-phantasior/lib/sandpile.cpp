#include "sandpile.h"
#include "parser.h"

#include <iostream>
#include <fstream>

void Sandpile::GetDataFromTSV() {
	std::ifstream in_file(input_path);

	if (in_file.fail()) {
		std::cerr << "Enable to open this file\n";
		exit(1);
	}

	std::string x;
	std::string y;
	std::string cnt;

	while (in_file >> x >> y >> cnt) {
		grid[stoi(x) - 1][stoi(y) - 1] = stoi(cnt);

		if (stoi(cnt) >= 4) {
			topple_queue.push({stoi(x) - 1, stoi(y) - 1});
		}
	}
}

void Sandpile::Resize() {
	bool need_resize_up = false;
	for (int i = 0; i < width; ++i) {
		if (grid[0][i] > 3) {
			need_resize_up = true;
			break;
		}
	}

	bool need_resize_down = false;
	for (int i = 0; i < width; ++i) {
		if (grid[length - 1][i] > 3) {
			need_resize_down = true;
			break;
		}
	}

	bool need_resize_left = false;
	for (int i = 0; i < length; ++i) {
		if (grid[i][0] > 3) {
			need_resize_left = true;
			break;
		}
	}

	bool need_resize_right = false;
	for (int i = 0; i < length; ++i) {
		if (grid[i].back() > 3) {
			need_resize_right = true;
			break;
		}
	}


	if (need_resize_up) {
		offset_y = 1;
		grid.push_front(std::deque<uint64_t>(width, 0));
		length++;
	}


	if (need_resize_down) {
		grid.push_back(std::deque<uint64_t>(width ,0));
		length++;
	}

	if (need_resize_left) {
		offset_x = 1;
		for (int i = 0; i < length; ++i) {
			grid[i].push_front(0);
		}

		width++;
	}

	if (need_resize_right) {
		for (int i = 0; i < length; ++i) {
			grid[i].push_back(0);
		}

		width++;
	}

}

// returns true if grid was changed;
bool Sandpile::Topple() {
	Resize();

	size_t iter_cnt = topple_queue.size();

	if (iter_cnt == 0) {
		return false; // grid is stable
	}

	for (int i = 0; i < iter_cnt; ++i) {
		size_t x = topple_queue.front().first  + offset_x;
		size_t y = topple_queue.front().second + offset_y;
		topple_queue.pop();

		grid[x][y] -= 4;
		grid[x - 1][y]++;
		grid[x + 1][y]++;
		grid[x][y - 1]++;
		grid[x][y + 1]++;

		if (grid[x][y] >= 4) {
			topple_queue.push({x, y});
		}

		if (grid[x - 1][y] == 4) {
			topple_queue.push({x - 1, y});
		}

		if (grid[x + 1][y] == 4) {
			topple_queue.push({x + 1, y});
		}

		if (grid[x][y - 1] == 4) {
			topple_queue.push({x, y - 1});
		}

		if (grid[x][y + 1] == 4) {
			topple_queue.push({x, y + 1});
		}
	}

	offset_x = 0;
	offset_y = 0;

	return true; // grid was changed
}
