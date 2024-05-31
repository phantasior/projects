#pragma once

#include <cinttypes>
#include <vector>

// [true] If you create archive from files, these files will be deleted!
// Also when you extract archive it will be deleted
const bool DEVELOP = true;

const uint32_t STAMP_SIZE = 9;

struct Stamp {
	uint8_t check;
    uint32_t block_size;
    uint32_t files_count;


    Stamp() {
    	check = 13;
        block_size = 0;
    	files_count = 0;
    }
}  __attribute__((packed, aligned(1)));

struct Header {
    uint32_t size;
    uint32_t name_length;

    Header() {
        size = 0;
        name_length = 0;
    }
} __attribute__((packed, aligned(1)));

bool CheckStamp(const Stamp& stamp);

void Create(char* archive_name, const std::vector<char*>& files_list, const uint32_t block_size);

void List(const char* archive_name);

void Extract(const char* archive_name, const std::vector<char*>& files_list);

void Append(const char* archive_name, const std::vector<char*>& files_list);

void Concat(const char* archive_name, const std::vector<char*>& files_list);

void Del(const char* archive_name, const std::vector<char*>& files_list);