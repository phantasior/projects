#include "connection_request.h"
#include "connection_setup.h"
#include "connection_setup_complete.h"

#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

int tx_send(uint8_t *buffer, ssize_t len) {
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

    return sock;
}

int main() {
    EncodedConnectionRequest request = CreateConnectionRequest();   

    printf("Send request\n");
    int sock = tx_send(request.ptr, request.size);

    uint8_t buff[128];
    int bytes_read = recv(sock, buff, sizeof(buff), 0);
    printf("Received setup\n Size: %d\n", bytes_read);

    RRCConnectionSetup_t* connection_setup = DecodeConnectionSetup(buff, bytes_read);
    printf("Connection setup ID: %ld\n", connection_setup->rrc_TransactionIdentifier);

    printf("Sending setup complete message");
    EncodedConnectionSetupComplete setup_complete = CreateConnectionSetupComplete();
    send(sock, setup_complete.ptr, setup_complete.size, 0);

    free(setup_complete.ptr);
    free(request.ptr);
    ASN_STRUCT_FREE(asn_DEF_RRCConnectionSetup, connection_setup);
    close(sock);
}