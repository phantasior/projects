#ifndef	_ConnectionRequest_H_
#define	_ConnectionRequest_H_

extern "C" {
    #include <RRCConnectionRequest.h>
}

typedef struct EncodedConnectionRequest {
    uint8_t* ptr;
    size_t size;
} EncodedConnectionRequest;

EncodedConnectionRequest CreateConnectionRequest();

RRCConnectionRequest_t* DecodeConnectionRequest(uint8_t* buffer, size_t size);

#endif