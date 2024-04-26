#include <mutex>
#include <memory>
#include <thread>
#include <vector>
#include <iostream>

int main() {
    // Вообще изначально это условие было в задаче со студентами, поэтому я там 
    // и написал код с синхронизацией, но раз поменяли то я такое дописал
    // (пример со студентами немного искуственный, но в целом корректный)

    std::mutex mtx;
    std::vector<int> arr;
    std::thread th([&]() {
        for (int i = 0; i < 10000; ++i) {
            std::lock_guard lock(mtx);
            arr.push_back(i);
        }
    });

    for (int i = 0; i < 10000; ++i) {
        std::lock_guard lock(mtx);
        if (!arr.empty()) {
            std::cout << arr.back() << '\n';
        }
    }

    th.join();
}