#include "connection_request.h"

#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>


EncodedConnectionRequest CreateConnectionRequest() {
    RRCConnectionRequest_t* connection_request = (RRCConnectionRequest_t *)calloc(1, sizeof(RRCConnectionRequest_t));

    if (!connection_request) {
        perror("calloc() failed");
        exit(1);
    }

    connection_request->criticalExtensions.present = RRCConnectionRequest__criticalExtensions_PR_rrcConnectionRequest_r8;
    connection_request->criticalExtensions.choice.rrcConnectionRequest_r8 = (RRCConnectionRequest_r8_IEs_t) {
        .spare = (BIT_STRING_t) { .size = 1, .buf = (uint8_t *)"q" },
        .ue_Identity = (InitialUE_Identity_t) {
            .present = InitialUE_Identity_PR_randomValue,
            .choice.randomValue = (BIT_STRING_t) { .size = 3, .buf = (uint8_t *)"abc" }
        },
        .establishmentCause = EstablishmentCause_mo_Signalling
    };


    const size_t buffer_size = 128;
    uint8_t* buffer = (uint8_t *)calloc(buffer_size, 1);
    asn_enc_rval_t er = asn_encode_to_buffer(0, ATS_BER, &asn_DEF_RRCConnectionRequest, connection_request, buffer, buffer_size);

    free(connection_request);

    return (EncodedConnectionRequest) {
        .ptr = buffer,
        .size = er.encoded
    };
}

RRCConnectionRequest_t* DecodeConnectionRequest(uint8_t* buffer, size_t size) {
    RRCConnectionRequest_t* request = 0;
    asn_dec_rval_t rval = asn_decode(0, ATS_BER, &asn_DEF_RRCConnectionRequest, (void **)&request, buffer, size);

    asn_fprint(stdout, &asn_DEF_RRCConnectionRequest, request);

    return request;
}