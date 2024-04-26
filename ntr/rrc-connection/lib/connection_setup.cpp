#include "connection_setup.h"

EncodedConnectionSetup CreateConnectionSetup() {
    RRCConnectionSetup_t* connection_setup = (RRCConnectionSetup_t *)calloc(1, sizeof(RRCConnectionSetup_t));

    if (!connection_setup) {
        perror("calloc() failed");
        exit(1);
    }

    connection_setup->rrc_TransactionIdentifier = 1;
    connection_setup->criticalExtensions.present = RRCConnectionSetup__criticalExtensions_PR_criticalExtensionsFuture;


    uint8_t* buffer = (uint8_t *)calloc(128, 1);
    asn_enc_rval_t er = asn_encode_to_buffer(0, ATS_BER, &asn_DEF_RRCConnectionSetup, connection_setup, buffer, 128);

    free(connection_setup);

    return (EncodedConnectionSetup) {
        .ptr = buffer,
        .size = static_cast<size_t>(er.encoded)
    };
}

RRCConnectionSetup_t* DecodeConnectionSetup(uint8_t* buffer, size_t size) {
    RRCConnectionSetup_t* setup = 0;
    asn_dec_rval_t rval = asn_decode(0, ATS_BER, &asn_DEF_RRCConnectionSetup, (void **)&setup, buffer, size);

    asn_fprint(stdout, &asn_DEF_RRCConnectionSetup, setup);

    return setup;
}