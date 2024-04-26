#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <iostream>

int main() {
    int sock, listener;
    struct sockaddr_in addr;
    char buf[1024];
    int bytes_read;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0) {
        perror("socket");
        std::exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(2);
    }

    listen(listener, 1);

    while(1) {
        sock = accept(listener, NULL, NULL);
        if(sock < 0) {
            perror("accept");
            exit(3);
        }

        while(1) {
            bytes_read = recv(sock, buf, 1024, 0);
            if(bytes_read <= 0) break;

            std::cout << "Recieved message\n";
            std::cout << bytes_read << "\n";

            char message[] = "Recieved your message";

            send(sock, message, sizeof(message), 0);
        }

        close(sock);
    }

    return 0;
}