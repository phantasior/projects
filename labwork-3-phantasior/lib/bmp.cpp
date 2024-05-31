#include "bmp.h"
#include "sandpile.h"

#include <iostream>
#include <fstream>


BMP::BMP(size_t width, size_t height) : width(width), height(height) {};


void BMP::Write(const Sandpile* sandpile, const char* path) const {
	std::ofstream out_file;
	out_file.open(path, std::ios::out | std::ios::binary);

	if (out_file.fail()) {
		std::cerr << "File could not be opened\n";
		exit(1);
	}

	unsigned char bmp_pad[3] = {0, 0, 0};
	const int PADDING = ((4 - (width) % 4) % 4);

	const int COLOR_TABLE_SIZE = 20;
	const int FILE_HEADER_SIZE = 14;
	const int INFORMATION_HEADER_SIZE = 40;
	const int FILE_SIZE = FILE_HEADER_SIZE + INFORMATION_HEADER_SIZE + COLOR_TABLE_SIZE + width * height;

	unsigned char file_header[FILE_HEADER_SIZE] = {0};

	// File type
	file_header[0] = 'B';
	file_header[1] = 'M';
	// File size
	file_header[2] = FILE_SIZE;
	file_header[3] = FILE_SIZE >> 8;
	file_header[4] = FILE_SIZE >> 16;
	file_header[5] = FILE_SIZE >> 24;
	// Pixel data offset
	file_header[10] = FILE_HEADER_SIZE + INFORMATION_HEADER_SIZE + COLOR_TABLE_SIZE;
	file_header[11] = (FILE_HEADER_SIZE + INFORMATION_HEADER_SIZE + COLOR_TABLE_SIZE) >> 8;
	file_header[12] = (FILE_HEADER_SIZE + INFORMATION_HEADER_SIZE + COLOR_TABLE_SIZE) >> 16;
	file_header[13] = (FILE_HEADER_SIZE + INFORMATION_HEADER_SIZE + COLOR_TABLE_SIZE) >> 24;

	unsigned char information_header[INFORMATION_HEADER_SIZE] = {0};

	// Header size
	information_header[0] = INFORMATION_HEADER_SIZE;
	// Bitmap width in pixels
	information_header[4] = width;
	information_header[5] = width >> 8;
	information_header[6] = width >> 16;
	information_header[7] = width >> 24;
	// Bitmap height in pixels
	information_header[8] = height;
	information_header[9] = height >> 8;
	information_header[10] = height >> 16;
	information_header[11] = height >> 24;
	// Number of color planes
	information_header[12] = 1;
	// Bits per pixel, color depth
	information_header[14] = 8;
	// Number of colors in color palette
	information_header[32] = 5;

	unsigned char color_table[COLOR_TABLE_SIZE] = {0};

	// White color
	color_table[0] = 255;
	color_table[1] = 255;
	color_table[2] = 255;
	color_table[3] = 1;

	// Green color
	color_table[4] = 0;
	color_table[5] = 255;
	color_table[6] = 0;
	color_table[7] = 1;

	// Violet color
	color_table[8] = 255;
	color_table[9] = 0;
	color_table[10] = 143;
	color_table[11] = 1;

	// Yellow color
	color_table[12] = 0;
	color_table[13] = 255;
	color_table[14] = 255;
	color_table[15] = 1;

	// Black color
	color_table[16] = 0;
	color_table[17] = 0;
	color_table[18] = 0;
	color_table[19] = 1;

	out_file.write(reinterpret_cast<char*>(file_header), FILE_HEADER_SIZE);
	out_file.write(reinterpret_cast<char*>(information_header), INFORMATION_HEADER_SIZE);
	out_file.write(reinterpret_cast<char*>(color_table), COLOR_TABLE_SIZE);

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int color_idx = 0;
			int sand = sandpile->grid[x][y];

			if (sand == 0) {
				color_idx = 0;
			} else if (sand == 1) {
				color_idx = 1;
			} else if (sand == 2) {
				color_idx = 2;
			} else if (sand == 3) {
				color_idx = 3;
			} else {
				color_idx = 4;
			}

			unsigned char color[1] = {static_cast<unsigned char>(color_idx)};

			out_file.write(reinterpret_cast<char*>(color), 1);

		}

		out_file.write(reinterpret_cast<char*>(bmp_pad), PADDING);
	}

	out_file.close();
}



void CreateSandpileBMP(const Sandpile* sandpile, uint32_t cur_iter) {
	BMP bmp(sandpile->width, sandpile->length);

	std::string file_name = static_cast<std::string>(sandpile->output_path) + 
							"sandpile_" + 
							std::to_string(cur_iter) + 
							".bmp";

	std::cout << "Created file: " << file_name << '\n';
	bmp.Write(sandpile, file_name.c_str());
}
