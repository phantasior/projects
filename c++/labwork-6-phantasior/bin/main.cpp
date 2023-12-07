#include "lib/parser.h"

#include <bits/stdc++.h>

using namespace OMFL;

int main(int, char**) {
    std::string data = R"(key1 = [1, true, 3.14, "ITMO", [1, 2, 3], ["a", "b", 28]])";
    const auto root = parse(data);

    std::cout << root.valid() << std::endl;
    std::cout << root.Get("key1")[0].IsInt() << std::endl;
}
