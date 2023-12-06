#pragma once

#include <cinttypes>
#include <vector>

// saves the current bits to be written in file
struct Buffer {
    std::vector<bool> buffer;
    FILE* file;
    uint32_t buf_end;

    Buffer();
    Buffer(const uint32_t block_size, FILE* _file);

    void Write(const std::vector<bool>& data);

    void WriteAll();

    void Print();
};

uint64_t ParityBitsRequired(const uint64_t& n_bits);

uint32_t ValidateEncodedData(std::vector<bool>& data, const uint32_t block_size);

void EncodeFile(const char* file_name);

void DecodeFile(const char* file_name);

void EncodeBlock(std::vector<bool>& data, std::vector<bool>& result, const uint32_t block_size);

void DecodeBlock(std::vector<bool>& data, std::vector<bool>& result, const uint32_t block_size);

void EncodeStamp(const char* file_name, Buffer& buffer);

Stamp DecodeStamp(FILE* archive);