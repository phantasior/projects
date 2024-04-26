#include "connection_request.h"
#include "connection_setup.h"
#include "connection_setup_complete.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#include <stdlib.h>

void tx_send(uint8_t *buffer, ssize_t len) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servaddr = {
        .sin_family = AF_INET,
        .sin_port = htons(3425),
        .sin_addr.s_addr = htonl(INADDR_LOOPBACK)
    };

    if(sock < 0) {
        perror("socket");
        exit(1);
    }

    if(connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect");
        exit(2);
    }

    send(sock, buffer, len, 0);

    close(sock);
}

int main() {
    struct sockaddr_in addr = (struct sockaddr_in) {
        .sin_family = AF_INET,
        addr.sin_port = htons(3425),
        addr.sin_addr.s_addr = htonl(INADDR_ANY)
    };

    uint8_t buf[1024];
    int listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0) {
        perror("socket");
        exit(1);
    }

    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(2);
    } 

    listen(listener, 1);

    int sock = accept(listener, NULL, NULL);
    if(sock < 0) {
        perror("accept");
        exit(3);
    }

    int bytes_read = recv(sock, buf, 1024, 0);
    printf("Received request\n Size: %d", bytes_read);

    if (bytes_read <= 0) {
        perror("bytes read < 0");
        exit(3);
    }

    RRCConnectionRequest_t* connection_request = DecodeConnectionRequest(buf, bytes_read);
    EncodedConnectionSetup connection_setup = CreateConnectionSetup();

    printf("Send connection setup\n");
    send(sock, connection_setup.ptr, connection_setup.size, 0);

    bytes_read = recv(sock, buf, 1024, 0);
    printf("Received connection setup complete\n Size: %d\n", bytes_read);

    if (bytes_read <= 0) {
        perror("bytes read < 0");
        exit(3);
    }

    RRCConnectionSetupComplete_t* setup_complete = DecodeConnectionSetupComplete(buf, bytes_read);
    printf("Setup complete ID: %ld\n", setup_complete->rrc_TransactionIdentifier);

    free(connection_setup.ptr);
    ASN_STRUCT_FREE(asn_DEF_RRCConnectionSetupComplete, setup_complete);
    ASN_STRUCT_FREE(asn_DEF_RRCConnectionRequest, connection_request);
    close(sock);
}