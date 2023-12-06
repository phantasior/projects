#include "arc.h"
#include "utils.h"
#include "hamming.h"

#include <iostream>
#include <fstream>

bool CheckStamp(const Stamp& stamp) {
	return stamp.check == 13 && sizeof(stamp) == sizeof(Stamp);
}

void Create(char* archive_name, const std::vector<char*>& files_list, const uint32_t block_size) {
    FILE* archive = fopen(archive_name, "wb");
    
    Stamp stamp;
    stamp.block_size = block_size;
    stamp.files_count = files_list.size();
    
    fwrite(&stamp, sizeof(Stamp), 1, archive);
    
    std::cout << "Archiving...\n";
    for (int i = 0; i < files_list.size(); i++) {
        Header header;
        header.size = GetFileSize(files_list[i]);
        header.name_length = strlen(files_list[i]);
        
        fwrite(&header, sizeof(Header), 1, archive);
        fwrite(files_list[i], header.name_length, 1, archive);
        
        uint8_t byte;
        FILE* file = fopen(files_list[i], "rb");
        
        if (!file) {
            Error("Aghhh again mann...");
        }
        
        std::cout << "\t->" << files_list[i] << '\n';
        
        for (int j = 0; j < header.size; j++) {
            fread(&byte, sizeof(uint8_t), 1, file);
            fwrite(&byte, sizeof(uint8_t), 1, archive);
        }

        fclose(file);
    }

    fclose(archive);
    EncodeFile(archive_name);

    if (DEVELOP) {
    	for (int i = 0; i < files_list.size(); ++i) {
    		remove(files_list[i]);
    	}
    }
}

void Extract(const char* archive_name, const std::vector<char*>& files_list) {
    DecodeFile(archive_name);

    FILE* archive = fopen(archive_name, "rb");

    if (!archive) {
    	Error("Why dude");
    }
    
    Stamp stamp;
    fread(&stamp, sizeof(Stamp), 1, archive);


    if (!CheckStamp(stamp)) {
    	Error("Unknown format");
    }

    if (stamp.files_count == 0) {
        Error("Empty archive");
    }
    
    std::cout << "Extracting...\n";
    for (int i = 0; i < stamp.files_count; i++) {
        Header header;
        fread(&header, sizeof(Header), 1, archive);
        
        uint8_t* file_name = new uint8_t[header.name_length + 1];
        uint8_t byte;
        
        for (int j = 0; j < header.name_length; j++) {
            fread(file_name + j, sizeof(uint8_t), 1, archive);
        }

        file_name[header.name_length] = '\0';

        bool is_extract = false;
        if (files_list.size() == 0) is_extract = true;

        for (int i = 0; i < files_list.size(); ++i) {
            if (strcmp(files_list[i], reinterpret_cast<char*>(file_name)) == 0) {
                is_extract = true;
                break;
            }
        }

        if (!is_extract) {
            fseek(archive, header.size, SEEK_CUR);
            delete[] file_name;
            continue;
        }

        FILE* file = fopen(reinterpret_cast<const char*>(file_name), "wb");
        
        std::cout << "\t->" << file_name << '\n';
        
        for (int j = 0; j < header.size; j++) {
            fread(&byte, sizeof(uint8_t), 1, archive);
            fwrite(&byte, sizeof(uint8_t), 1, file);
        }
        
        delete[] file_name;

        fclose(file);
    }
    
    fclose(archive);

    if (DEVELOP) {
    	remove(archive_name);
    }
}

void List(const char* archive_name) {
    DecodeFile(archive_name);
    FILE* archive = fopen(archive_name, "rb");

    if (!archive) {
        Error("cant open sry");
    }

    Stamp stamp;
    fread(&stamp, sizeof(Stamp), 1, archive);

    if (!CheckStamp(stamp)) {
        Error("Unknown format");
    }

    std::cout << "Number of files: " << stamp.files_count << '\n';

    std::cout << "List of files in archive:\n";

    for (int i = 0; i < stamp.files_count; ++i) {
        Header header;
        fread(&header, sizeof(Header), 1, archive);

        uint8_t* file_name = new uint8_t[header.name_length + 1];

        for (int j = 0; j < header.name_length; ++j) {
            fread(file_name + j, sizeof(uint8_t), 1, archive);
        }

        file_name[header.name_length] = '\0';

        std::cout << file_name << '\n';

        fseek(archive, header.size, SEEK_CUR);

        delete[] file_name;
    }

    fclose(archive);
    EncodeFile(archive_name);
}

void Append(const char* archive_name, const std::vector<char*>& files_list) {
    DecodeFile(archive_name);
    FILE* archive = fopen(archive_name, "r+wb");

    if (!archive) {
        Error("cant open sry");
    }

    Stamp stamp;
    fread(&stamp, sizeof(Stamp), 1, archive);

    if (!CheckStamp(stamp)) {
        Error("Unknown format");
    }

    fseek(archive, 0, SEEK_SET);
    stamp.files_count += files_list.size();
    fwrite(&stamp, sizeof(stamp), 1, archive);
    fseek(archive, 0, SEEK_END);

    std::cout << "Appending files:\n";

    for (int i = 0; i < files_list.size(); ++i) {
        Header header;
        header.size = GetFileSize(files_list[i]);
        header.name_length = strlen(files_list[i]);
        
        fwrite(&header, sizeof(Header), 1, archive);
        fwrite(files_list[i], header.name_length, 1, archive);
 
        uint8_t byte;

        FILE* file = fopen(files_list[i], "rb");
        
        if (!file) {
            Error("Aghhh again mann...");
        }
        
        std::cout << "\t->" << files_list[i] << '\n';
        
        for (int j = 0; j < header.size; j++) {
            fread(&byte, sizeof(uint8_t), 1, file);
            fwrite(&byte, sizeof(uint8_t), 1, archive);
        }

        fclose(file);
    }

    if (DEVELOP) {
        for (int i = 0; i < files_list.size(); ++i) {
            remove(files_list[i]);
        }
    }

    fclose(archive);
    EncodeFile(archive_name);
}

void Concat(const char* archive_name, const std::vector<char*>& files_list) {
    FILE* archive = fopen(archive_name, "wb");
    
    Stamp stamp;
    fseek(archive, sizeof(Stamp), SEEK_SET);
    
    std::cout << "Merging archives...\n";
    for (int i = 0; i < files_list.size(); i++) {
        FILE* cur_file = fopen(files_list[i], "rb");

        if (!cur_file) {
            Error("cant open sry");
        }

        std::cout << "\t->" << files_list[i] << '\n';

        Stamp cur_stamp;
        fread(&cur_stamp, sizeof(Stamp), 1, cur_file);

        if (stamp.block_size && stamp.block_size != cur_stamp.block_size) {
            fclose(archive);
            remove(archive_name);
            Error("Can't concatanate archives with different block size");
        }

        if (stamp.block_size == 0) {
            stamp.block_size = cur_stamp.block_size;
        }

        if (!CheckStamp(cur_stamp)) {
            Error("Unknown format");
        }

        for (int j = 0; j < cur_stamp.files_count; ++j) {
            Header cur_header;
            fread(&cur_header, sizeof(Header), 1, cur_file);
            fwrite(&cur_header, sizeof(Header), 1, archive);

            uint8_t byte;

            for (int k = 0; k < cur_header.name_length; ++k) {
                fread(&byte, sizeof(uint8_t), 1, cur_file);
                fwrite(&byte, sizeof(uint8_t), 1, archive);
            }

            for (int k = 0; k < cur_header.size; ++k) {
                fread(&byte, sizeof(uint8_t), 1, cur_file);
                fwrite(&byte, sizeof(uint8_t), 1, archive);
            }

            stamp.files_count++;
        }

        fclose(cur_file);
    }

    fseek(archive, 0, SEEK_SET);
    fwrite(&stamp, sizeof(stamp), 1, archive);
    
    fclose(archive);
    EncodeFile(archive_name);

    if (DEVELOP) {
        for (int i = 0; i < files_list.size(); ++i) {
            remove(files_list[i]);
        }
    }
}

void Del(const char* archive_name, const std::vector<char*>& files_list) {
    DecodeFile(archive_name);
    FILE* archive = fopen(archive_name, "rb");

    if (!archive) {
        Error("Cant open file agh");
    }

    Stamp stamp;
    fread(&stamp, sizeof(Stamp), 1, archive);

    if (!CheckStamp(stamp)) {
        Error("Unknown format");
    }

    FILE* new_archive = fopen(temp_archive_name, "wb");

    stamp.files_count -= files_list.size();
    fwrite(&stamp, sizeof(stamp), 1, new_archive);

    std::cout << "Deleting files:\n";
    for (size_t i = 0; i < stamp.files_count + files_list.size(); ++i) {
        Header header;
        fread(&header, sizeof(Header), 1, archive);
        
        uint8_t* file_name = new uint8_t[header.name_length + 1];
        
        for (size_t j = 0; j < header.name_length; j++) {
            fread(file_name + j, sizeof(uint8_t), 1, archive);
        }

        file_name[header.name_length] = '\0';

        bool is_delete = false;

        for (size_t j = 0; j < files_list.size(); ++j) {
            if (!strcmp(files_list[j], reinterpret_cast<char*>(file_name))) {
                is_delete = true;
                break;
            }
        }

        if (is_delete) {
            std::cout << "\t->" << file_name << '\n';
            fseek(archive, header.size, SEEK_CUR);
        } else {
            fwrite(&header, sizeof(header), 1, new_archive);
            fwrite(file_name, header.name_length, 1, new_archive);
            uint8_t byte;

            for (int j = 0; j < header.size; ++j) {
                fread(&byte, sizeof(uint8_t), 1, archive);
                fwrite(&byte, sizeof(uint8_t), 1, new_archive);
            }
        }

        delete[] file_name;
    }

    fclose(archive);
    fclose(new_archive);

    remove(archive_name);
    rename(temp_archive_name, archive_name);

    EncodeFile(archive_name);
}
