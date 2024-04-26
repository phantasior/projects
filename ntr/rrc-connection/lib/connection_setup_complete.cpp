#include "connection_setup_complete.h"

EncodedConnectionSetupComplete CreateConnectionSetupComplete() {
    RRCConnectionSetupComplete_t* connection_setup_complete = (RRCConnectionSetupComplete_t *)calloc(1, sizeof(RRCConnectionSetupComplete_t));

    if (!connection_setup_complete) {
        perror("calloc() failed");
        exit(1);
    }

    connection_setup_complete->rrc_TransactionIdentifier = 1;
    connection_setup_complete->criticalExtensions.present = RRCConnectionSetupComplete__criticalExtensions_PR_criticalExtensionsFuture;


    uint8_t* buffer = (uint8_t *)calloc(128, 1);
    asn_enc_rval_t er = asn_encode_to_buffer(0, ATS_BER, &asn_DEF_RRCConnectionSetupComplete, connection_setup_complete, buffer, 128);

    free(connection_setup_complete);

    return (EncodedConnectionSetupComplete) {
        .ptr = buffer,
        .size = static_cast<size_t>(er.encoded)
    };
}

RRCConnectionSetupComplete_t* DecodeConnectionSetupComplete(uint8_t* buffer, size_t size) {
    RRCConnectionSetupComplete_t* setup_complete = 0;
    asn_dec_rval_t rval = asn_decode(0, ATS_BER, &asn_DEF_RRCConnectionSetupComplete, (void **)&setup_complete, buffer, size);

    asn_fprint(stdout, &asn_DEF_RRCConnectionSetupComplete, setup_complete);

    return setup_complete;
}