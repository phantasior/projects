#include <lib/number.h>
#include <iostream>

int main() {
    uint2022_t a = from_string("123456789");
    uint2022_t b = from_string("987654321");

    std::cout << a << ' ' << b << '\n';

    std::cout << a + b << '\n' << b - a <<   '\n' << a * b << '\n' << b / a << '\n';

    /*
    123456789 987654321
    1111111110
    864197532
    121932631112635269
    8
    */
}
