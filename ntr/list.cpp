#include <iostream>

namespace STD {

template<typename T>
struct Node {
    Node(T value) : value(std::move(value)), next(nullptr) {}

    bool operator==(const Node& other) const {
        return value == other.value;
    }

    T value;
    Node* next;
};

template<typename T>
class Iterator {
public:
    using key_type = T;
    using value_type = Node<key_type>;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using difference_type = std::ptrdiff_t;
    using size_type = unsigned int;
    using iterator_category = std::forward_iterator_tag;

    Iterator& operator++() {
        ptr_ = ptr_->next;
        return *this;
    }

    Iterator operator++(int) {
        auto it = *this;
        return ++it;
    }

    Iterator(pointer ptr) : ptr_(ptr) { }

    T& operator*() {
        return ptr_->value;
    }

    pointer operator->() {
        return ptr_;
    }

    bool operator==(const Iterator& other) const {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const Iterator& other) const {
        return !(*this == other);
    }

private:
    pointer ptr_;
};

template<typename T>
class ConstIterator {
public:
    using key_type = T;
    using value_type = Node<key_type>;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using difference_type = std::ptrdiff_t;
    using size_type = unsigned int;
    using iterator_category = std::forward_iterator_tag;

    ConstIterator& operator++() {
        ptr_ = ptr_->next;
        return *this;
    }

    ConstIterator operator++(int) {
        auto it = *this;
        return ++it;
    }

    ConstIterator(const_pointer ptr) : ptr_(ptr) { }

    const_reference operator*() const {
        return ptr_->value;
    }

    pointer operator->() const {
        return ptr_;
    }

    bool operator==(const ConstIterator& other) const {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const ConstIterator& other) const {
        return !(*this == other);
    }

private:
    const_pointer ptr_;
};

template<typename T, typename Allocator = std::allocator<T>>
class List {
public:
    using value_type = T;
    using allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<Node<T>>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
    using allocator_traits = typename std::allocator_traits<allocator_type>;

    using iterator = Iterator<T>;
    using const_iterator = ConstIterator<T>;

    List() { }

    List(std::initializer_list<T> data) {
        for (const auto& el : data) {
            this->push_back(el);
        }
    }

    List(const List& other) 
        : allocator_(other.allocator_) {
        head_ = ConstructNode(*other.head_);
        tail_ = head_;
        pointer runner = other.head_->next;
        while (runner) {
            tail_->next = ConstructNode(*runner);
            runner = runner->next;
            tail_ = tail_->next;
        }
    }

    List& operator=(List other) {
        swap(*this, other);
        return *this;
    }

    List(List&& other) noexcept : List() {
        swap(*this, other);
    }

    ~List() {
        ClearList();
    }

    void push_back(const_reference value) {
        if (TryInsertIntoEmptyList(value)) {
            return;
        }

        tail_->next = ConstructNode(value);
        tail_ = tail_->next;
    }

    void push_back(T&& value) {
        if (TryInsertIntoEmptyList(std::move(value))) {
            return;
        }
        
        tail_->next = ConstructNode(std::move(value));
        tail_ = tail_->next;
    }

    void push_front(const_reference value) {
       if (TryInsertIntoEmptyList(value)) {
            return;
        }

        pointer mem = head_;
        head_ = ConstructNode(value);
        head_->next = mem;
    }

    void push_front(T&& value) {
        if (TryInsertIntoEmptyList(std::move(value))) {
            return;
        }

        pointer mem = head_;
        head_ = ConstructNode(std::move(value));
        head_->next = mem;
    }

    void insert_after(iterator it, const_reference value) {
        pointer mem = it->next;
        it->next = ConstructNode(value);
        it->next->next = mem;
    }

    void insert_after(iterator it, T&& value) {
        pointer mem = it->next;
        it->next = ConstructNode(std::move(value));
        it->next->next = mem;
    }
     
    void erase_after(iterator it) {
        if (it->next == tail_->next) {
            return;
        }

        pointer mem = it->next;
        it->next = it->next->next;
        DestroyNode(mem);
    }

    iterator find(const T& value) {
        return std::find_if(begin(), end(), [&](auto&& cur) {
            return cur == value;
        });
    }

    iterator begin() {
        return iterator(head_);
    }

    const_iterator cbegin() const {
        return const_iterator(head_);
    }

    iterator end() {
        return iterator(tail_->next);
    }

    const_iterator cend() const {
        return const_iterator(tail_->next);
    }

    bool operator==(const List& other) const {
        auto it = other.cbegin();
        auto it2 = cbegin();

        while (it != other.cend() && it2 != cend()) {
            if (*it != *it2) {
                return false;
            }

            ++it; ++it2;
        }

        return it == other.cend() && it2 == cend();
    }

    void swap(List& other) {
        using std::swap;

        swap(head_, other.head_);
        swap(tail_, other.tail_);
        swap(allocator_, other.allocator_);
    }

    template<typename U>
    friend std::ostream& operator<<(std::ostream& out, List<U>& list) {
        for (const auto& el : list) {
            out << el << ' ';
        }

        out << '\n';
        return out;
    }

private:
    template<typename ...Args>
    pointer ConstructNode(Args&&... args) {
        pointer node = allocator_traits::allocate(allocator_, 1);
        allocator_traits::construct(allocator_, node, std::forward<Args...>(args...));
        return node;
    }

    void DestroyNode(pointer val) {
        allocator_traits::destroy(allocator_, val);
        allocator_traits::deallocate(allocator_, val, 1);
    }

    void ClearList() {
        while (head_) {
            pointer next = head_->next;
            DestroyNode(head_);
            head_ = next;
        }
    }

    bool TryInsertIntoEmptyList(T&& value) {
        if (head_ != nullptr) {
            return false;
        }

        head_ = ConstructNode(std::move(value));
        tail_ = head_;
        return true;
    }

        bool TryInsertIntoEmptyList(const_reference value) {
        if (head_ != nullptr) {
            return false;
        }

        head_ = ConstructNode(value);
        tail_ = head_;
        return true;
    }

    Node<T>* head_ = nullptr;
    Node<T>* tail_ = nullptr;
    [[no_unique_address]] allocator_type allocator_;
};

} // namespace STD

void InsertTests() {
    STD::List<int> list;

    list.push_back(3);
    list.push_back(4);
    list.push_back(6);

    list.insert_after(++list.begin(), 5);

    list.push_front(2);
    list.push_front(1);

    std::cout << list;
}

void DeleteTests() {
    STD::List<int> before = {1, 2, 3, 4, 5};
    STD::List<int> after = before;

    after.erase_after(after.begin());

    std::cout << after;

    assert(after == STD::List<int>({2, 3, 4, 5}));
}

void FindTests() {
    STD::List<int> list = {1, 2, 3, 4, 5};

    assert(list.find(3) != list.end());
    assert(list.find(1) == list.begin());
}

int main() {
    InsertTests();
    DeleteTests();
    FindTests();
}