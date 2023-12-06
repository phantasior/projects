#include <iostream>
#include<fstream>
#include<string>
#include <cstring>


struct Options {
    bool lines;
    bool bytes;
    bool words;
    bool chars;

    Options() {
        lines = false;
        bytes = false;
        words = false;
        chars = false;
    }

    bool is_used() const {
        return lines || bytes || words || chars;
    }
};


size_t GetFileSize(const std::string& path_to_file) {
    std::ifstream in_file(path_to_file, std::ifstream::ate | std::ifstream::binary);

    if (!in_file.is_open()) {
        std::cout << "Failed to open " << path_to_file << '\n';
        exit(1);
    }

    return in_file.tellg();
}

void WordCount(const std::string& path_to_file, const Options& option) {
    std::ifstream in_file(path_to_file);

    if (!in_file.is_open()) {
        std::cout << "Failed to open " << path_to_file << '\n';
        exit(1);
    }

    size_t number_of_bytes = GetFileSize(path_to_file);
    size_t number_of_lines = 0;
    size_t number_of_words = 0;
    size_t number_of_chars = 0;

    char previous_char = ' ';
    char current_char;
    
    while (in_file.get(current_char)) {
        if (current_char == '\n') 
            number_of_lines++;

        if (isgraph(current_char) || current_char == '\t') 
            number_of_chars++;

        if (isspace(current_char) && !isspace(previous_char)) 
            number_of_words++;

        previous_char = current_char;
    }

    if (!isspace(previous_char)) 
        number_of_words++;


    //if user didn't select any option
    if (option.is_used() == false) {
        std::cout << number_of_lines << " " 
                << number_of_words << " " 
                << number_of_bytes << " " 
                << path_to_file << '\n';
    } else {
        if (option.lines) 
            std::cout << number_of_lines << ' ';

        if (option.words) 
            std::cout << number_of_words << ' ';

        if (option.bytes) 
            std::cout << number_of_bytes << ' ';

        if (option.chars) 
            std::cout << number_of_chars << ' ';

        std::cout << path_to_file << '\n';
    }
}


void HandleOptions(const char* arg, Options& option) {
    if (strcmp(arg, "-l") == 0 || strcmp(arg, "--lines") == 0) {
        option.lines = true;
    } else if (strcmp(arg, "-c") == 0 || strcmp(arg, "--bytes") == 0) {
        option.bytes = true;
    } else if (strcmp(arg, "-w") == 0 || strcmp(arg, "--words") == 0) {
        option.words = true;
    } else if (strcmp(arg, "-m") == 0 || strcmp(arg, "--chars") == 0) {
        option.chars = true;
    }
}


int main(int argc, char** argv) {
    Options option;

    for (int i = 1; i < argc; ++i) {
        char* arg = argv[i];

        if (arg[0] == '-') {
            HandleOptions(arg, option);
        } else {
            WordCount(arg, option);
        }
    }

    return 0;
}
