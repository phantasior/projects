#include "../lib/algo.hpp"

#include <iostream>
#include <vector>
#include <tuple>
#include <forward_list>
#include <algorithm>

using std::cout;
using std::endl;

using namespace std;

struct S {

};;

int main() {
    double k = 1.5;
            k++;
    cout << k << endl;
    for (const auto i : algo::range(1.5, 5.5)) {
        cout << i << ' ';
    }

}
