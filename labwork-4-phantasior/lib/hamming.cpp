#include "arc.h"
#include "utils.h"
#include "hamming.h"

#include <iostream>

Buffer::Buffer(const uint32_t block_size, FILE* _file) {
    buffer.assign(3 * block_size, false);
    file = _file;
    buf_end = 0;
}

// expand buffer and write 8bits to file, delete them from buffer
void Buffer::Write(const std::vector<bool>& data) {
    for (int i = 0; i < data.size(); ++i) {
        buffer[i + buf_end] = data[i];
    }

    buf_end += data.size();
    uint32_t shift = 0;

    for (int i = 0; i + 8 <= buf_end; i += 8) {
        uint8_t byte = 0;
        shift = i + 8;

        for (int j = 0; j < 8; ++j) {
            if (buffer[i + j] == true) {
                byte |= (1 << (7 - j));
            }

        }

        fwrite(&byte, sizeof(uint8_t), 1, file);
    }

    for (int i = shift; i < buf_end; ++i) {
        buffer[i - shift] = buffer[i];
    }

    buf_end -= shift;
}

void Buffer::WriteAll() {
    uint8_t byte = 0;

    for (size_t i = 0; i < buf_end; ++i) {
        if (buffer[i] == true) {
                byte |= (1 << (8 - i - 1));
        }
    }

    fwrite(&byte, sizeof(uint8_t), 1, file);
    buf_end = 0;
}

void Buffer::Print() {
    std::cout << "Buf_end: " << buf_end << '\n';
    for (int i = 0; i < buf_end; ++i) std::cout << buffer[i];
    std::cout << '\n'; 
}

uint32_t ValidateEncodedData(std::vector<bool>& data, const uint32_t block_size) {
    size_t parity_bits = ParityBitsRequired(block_size);
    size_t redundant_size = parity_bits + block_size;

    std::vector<bool> check(parity_bits, 0);

    size_t cur_check_idx = 0;
    for (size_t i = 1; i < redundant_size; i *= 2) {
        size_t parity = 0;

        size_t cur_ptr = i - 1;

        while (cur_ptr < redundant_size) {
            for (int j = cur_ptr; j < cur_ptr + i && j < redundant_size; ++j) {
                parity += data[j];
            }

            cur_ptr += i * 2;
        }

        check[cur_check_idx++] = parity % 2;
    }

    size_t error_bit = 0;
    unsigned long long cur_pow = 1;
    for (int i = 0; i < parity_bits; ++i) {
        error_bit += cur_pow * check[i];
        cur_pow *= 2;
    }

    return error_bit;
}

uint64_t ParityBitsRequired(const uint64_t& n_bits) {
    uint64_t parity_bits = 1;
    uint64_t cur_pow = 2;

    while (true) {
        uint64_t lhs = cur_pow;
        uint64_t rhs = parity_bits + n_bits + 1;
        if (lhs >= rhs) {
            break;
        }

        parity_bits++;
        cur_pow *= 2;
    }

    return parity_bits;
}

void EncodeBlock(std::vector<bool>& data, std::vector<bool>& result, const uint32_t block_size) {
    size_t parity_bits = ParityBitsRequired(block_size);
    size_t redundant_size = parity_bits + block_size;

    long long cur_pow = 1;
    size_t bset_ptr = 0;
    for (size_t i = 0; i < redundant_size; ++i) {
        if (i == cur_pow - 1) {
            result[i] = 0;
            cur_pow *= 2;
        } else {
            result[i] = data[bset_ptr++];
        }
    }

    for (size_t i = 1; i <= redundant_size; i *= 2) {
        size_t parity = 0;

        size_t cur_ptr = i - 1;

        while (cur_ptr < redundant_size) {
            for (int j = cur_ptr; j < cur_ptr + i && j < redundant_size; ++j) {
                parity += result[j];
            }

            cur_ptr += i * 2;
        }

        result[i - 1] = parity % 2;
    }
}

void DecodeBlock(std::vector<bool>& data, std::vector<bool>& result, const uint32_t block_size) {
    size_t parity_bits = ParityBitsRequired(block_size);
    size_t redundant_size = parity_bits + block_size;

    uint32_t error_bit = ValidateEncodedData(data, block_size);

    if (error_bit != 0) {
        // std::cout << "Found wrong bit at position " << error_bit << " however it was fixed\n";
        data[error_bit - 1] = 1 - data[error_bit - 1];
        
        if (ValidateEncodedData(data, block_size) == 0) {
            std::cout << "Fixed bit at position " << error_bit << '\n';
        } else {
            std::cerr << "File was damaged, impossible to fix it :(\n";
        }
    }

    uint32_t cur_pow = 1;
    size_t cur_idx = 0;
    for (size_t i = 0; i < redundant_size; ++i) {
        if (i == cur_pow - 1) {
            cur_pow *= 2;
        } else {
            result[cur_idx++] = data[i];
        }
    }
}

// return the left part of encoded Stamp
void EncodeStamp(const char* file_name, Buffer& buffer) {
    size_t stamp_size = sizeof(Stamp);
    uint32_t parity_stamp_bits = ParityBitsRequired(stamp_size * 8);
    uint32_t redundant_stamp_size = parity_stamp_bits + stamp_size * 8;
    uint8_t byte;

    std::vector<bool> stamp_data(stamp_size * 8, 0);
    std::vector<bool> encoded_stamp_data(redundant_stamp_size, 0);

    FILE* archive = fopen(file_name, "rb");

    for (int i = 0; i < stamp_size; ++i) {
        fread(&byte, sizeof(uint8_t), 1, archive);
        
        for (int j = 0; j < 8; ++j) {
            stamp_data[i * 8 + j] = (byte >> (7 - j)) & 1;
        }
    }

    EncodeBlock(stamp_data, encoded_stamp_data, stamp_size * 8);
    buffer.Write(encoded_stamp_data);

    fclose(archive);
}

void EncodeFile(const char* file_name) {
    FILE* archive = fopen(file_name, "rb");

    Stamp stamp;
    fread(&stamp, sizeof(Stamp), 1, archive);

    if (!CheckStamp(stamp)) {
    	Error("Unknown format");
    }

    FILE* tmp_file = fopen(temp_archive_name, "wb");
    uint32_t block_size = stamp.block_size;

    Buffer buffer(sizeof(Stamp) * 8, tmp_file);

    EncodeStamp(file_name, buffer);


    uint32_t parity_bits = ParityBitsRequired(block_size);
    uint32_t redundant_size = parity_bits + block_size;
    uint8_t byte;

    std::vector<bool> data_buffer(block_size, 0);
    std::vector<bool> coded_data_buffer(redundant_size, 0);

    uint32_t block_ptr = 0;
    uint32_t byte_ptr = 0;

    // reading file byte by byte
    while (fread(&byte, sizeof(uint8_t), 1, archive) == 1) {
        byte_ptr = 0;

        while (byte_ptr < 8) {
            if (block_ptr == block_size) {
                EncodeBlock(data_buffer, coded_data_buffer, block_size);
                // buffer.Print();
                buffer.Write(coded_data_buffer);
                block_ptr = 0;
            }

            data_buffer[block_ptr] = (byte >> (7 - byte_ptr)) & 1;
            block_ptr++;
            byte_ptr++;
        }

    }

    buffer.WriteAll();

    fclose(archive);
    fclose(tmp_file);

    remove(file_name);
    rename(temp_archive_name, file_name);
}

Stamp DecodeStamp(FILE* archive) {
    size_t stamp_size = sizeof(Stamp);
    uint32_t parity_stamp_bits = ParityBitsRequired(stamp_size * 8);
    uint32_t redundant_stamp_size = parity_stamp_bits + stamp_size * 8;
    uint8_t byte;

    std::vector<bool> stamp_data(stamp_size * 8, 0);
    std::vector<bool> encoded_stamp_data(redundant_stamp_size, 0);

    for (int i = 0; i < redundant_stamp_size / 8; ++i) {
        fread(&byte, sizeof(uint8_t), 1, archive);

        for (int j = 0; j < 8; ++j) {
            encoded_stamp_data[i * 8 + j] = (byte >> (7 - j)) & 1;
        }
    }

    fread(&byte, sizeof(uint8_t), 1, archive);
    for (int i = redundant_stamp_size - (redundant_stamp_size % 8); i < redundant_stamp_size; ++i) {
        encoded_stamp_data[i] = (byte >> (7 - (i % 8))) & 1;
    }

    DecodeBlock(encoded_stamp_data, stamp_data, stamp_size * 8);

    FILE* tmp_file = fopen(temp_archive_name, "wb");

    Buffer tmp_buffer(sizeof(Stamp) * 8, tmp_file);
    tmp_buffer.Write(stamp_data);

    fclose(tmp_file);
    tmp_file = fopen(temp_archive_name, "rb");

    Stamp stamp;
    fread(&stamp, sizeof(Stamp), 1, tmp_file);
    fclose(tmp_file);

    return stamp;
}

void DecodeFile(const char* file_name) {
    FILE* archive = fopen(file_name, "rb");

    Stamp stamp = DecodeStamp(archive);

    if (!CheckStamp(stamp)) {
    	Error("Unknown format");
    }


    const uint32_t block_size = stamp.block_size;

    FILE* tmp_file = fopen(temp_archive_name, "wb");
    fwrite(&stamp, sizeof(Stamp), 1, tmp_file);

    uint32_t parity_bits = ParityBitsRequired(block_size);
    uint32_t redundant_size = parity_bits + block_size;

    Buffer buffer(sizeof(Stamp) * 8, tmp_file);

    std::vector<bool> data_buffer(block_size, 0);
    std::vector<bool> coded_data_buffer(redundant_size, 0);

    uint32_t block_ptr = 0;
    uint32_t byte_ptr = 0;
    uint8_t byte;

    fseek(archive, -1, SEEK_CUR);

    size_t stamp_size = sizeof(Stamp);
    uint32_t parity_stamp_bits = ParityBitsRequired(stamp_size * 8);
    uint32_t redundant_stamp_size = parity_stamp_bits + stamp_size * 8;

    fread(&byte, sizeof(uint8_t), 1, archive);
    for (int i = redundant_stamp_size % 8; i < 8; ++i) {
        coded_data_buffer[block_ptr++] = (byte >> (7 - (i % 8))) & 1;
    }

    // reading file byte by byte
    while (fread(&byte, sizeof(uint8_t), 1, archive) == 1) {
        byte_ptr = 0;

        while (byte_ptr < 8) {
            if (block_ptr == redundant_size) {
                DecodeBlock(coded_data_buffer, data_buffer, block_size);
                buffer.Write(data_buffer);
                block_ptr = 0;
            }

            coded_data_buffer[block_ptr] = (byte >> (7 - byte_ptr)) & 1;
            block_ptr++;
            byte_ptr++;
        }
    }

    buffer.Write(data_buffer);
    buffer.WriteAll();

    fclose(archive);
    fclose(tmp_file);

    remove(file_name);
    rename(temp_archive_name, file_name);
}
