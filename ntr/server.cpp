#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <iostream>

template <class T> 
class CircularBuffer { 
public:
  CircularBuffer(size_t max_size)
        : buffer(std::make_unique<T[]>(max_size))
        , max_size(max_size)
    { }

    void enqueue(T item) {
        if (is_full())
            throw std::runtime_error("buffer is full");

        buffer[tail] = std::move(item);
        tail = (tail + 1) % max_size;
    }

    T dequeue() {
        if (is_empty())
            throw std::runtime_error("buffer is empty");

        T item = buffer[head];

        buffer[head] = empty_item;
        head = (head + 1) % max_size;

        return item;
    }

    T front() const { return buffer[head]; }

    bool is_empty() const { return head == tail; }

    bool is_full() const { return tail == (head - 1) % max_size; }

    size_t size() const {
        if (tail >= head)
            return tail - head;

        return max_size - head - tail;
  }

private:
    std::unique_ptr<T[]> buffer;
    size_t head = 0;
    size_t tail = 0;
    size_t max_size;
    T empty_item;
};

int main() {
    const size_t kMaxSize = 1024;
    CircularBuffer<uint8_t> buffer(kMaxSize);
    uint8_t buf[kMaxSize];

    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0) {
        perror("socket");
        std::exit(1);
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(3425),
        .sin_addr.s_addr = htonl(INADDR_ANY)
    };

    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(2);
    }

    listen(listener, 1);

    while(1) {
        int sock = accept(listener, NULL, NULL);
        if(sock < 0) {
            perror("accept");
            exit(3);
        }

        while(1) {
            int bytes_read = recv(sock, buf, kMaxSize, 0);
            if(bytes_read <= 0) break;

            for (int i = 0; i < bytes_read; ++i) {
                buffer.enqueue(buf[i]);
            }

            std::cout << "Recieved message\n";
            std::cout << bytes_read << "\n";

            char message[] = "Recieved your message";

            send(sock, message, sizeof(message), 0);
        }

        close(sock);
    }

    return 0;
}