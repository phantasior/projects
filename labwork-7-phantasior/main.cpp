#include "./lib/parser.hpp"

#include <iostream>

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cout << "No file found\n";
    } else {
        Parse(argv[1]);        
    }
}