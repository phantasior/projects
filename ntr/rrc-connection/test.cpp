#include <string>
#include <map>
#include <memory>
#include <fstream>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>

struct Id {
    Id() {
        static size_t current = 0;
        value = current++;
    }

    Id(int value) : value(value) {}

    bool operator<(const Id& other) const {
        return value < other.value;
    }

    friend std::ostream& operator<<(std::ostream& out, const Id& id) {
        out << id.value << '\n';
        return out;
    }

    size_t value;
};

struct Student {
    Student(size_t age, const std::string& name)
        : age(age)
        , name(name)
    {}

    Id id;
    size_t age;
    std::string name;

    friend std::ostream& operator<<(std::ostream& out, const Student& student) {
        out << "Student = {\n";
        out << "    .age = " << student.age << '\n';
        out << "    .name = " << student.name << '\n';
        out << "};\n";

        return out;
    }
};

class StudentSet {
public:
    Id insert(Student student) {
        std::unique_lock lock(mtx_);

        auto insert_ret = students_.insert({student.id, std::make_shared<Student>(std::move(student))});

        cv_wait_student_.notify_all();
        return insert_ret.first->first;
    }

    template<typename ...Args>
    Id emplace(Args&& ...args) {
        std::unique_lock lock(mtx_);

        std::shared_ptr<Student> student = std::make_shared<Student>(std::forward<Args>(args)...);
        auto insert_ret = students_.insert({student->id, student});

        cv_wait_student_.notify_all();
        return insert_ret.first->first;
    }

    std::shared_ptr<Student> get(Id id) const {
        std::unique_lock lock(mtx_);
        cv_wait_student_.wait(lock, [&] { return students_.contains(id); });
        return students_.at(id);
    }

    void erase(Id id) {
        std::lock_guard lock(mtx_);
        students_.erase(id);
    }
private:
    mutable std::mutex mtx_;
    mutable std::condition_variable cv_wait_student_;
    std::map<Id, std::shared_ptr<Student>> students_; // Guarded by mtx
};

int main() {
    StudentSet set;

    std::thread th([&](){
        for (int i = 0; i < 100; ++i) {
            set.emplace(i, "name");
        }
    });

    for (int i = 0; i < 100; ++i) {
        std::cout << *set.get(static_cast<Id>(i));
    }

    th.join();
}