#include <memory>
#include <iostream>

int main() {
    int32_t arr[10];
    std::cout << (arr[4]) << '\n';
    std::cout << *(arr + 4) << '\n';
}