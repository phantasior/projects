#include "lib/parser.h"

#include <iostream>

using namespace OMFL;

int main(int, char**) {
    std::string data = R"(
        key = 
        "value"
    )";
    const auto root = parse(data);

    std::cout << root.valid() << std::endl;
}
