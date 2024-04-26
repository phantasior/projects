#ifndef	_ConnectionSetup_H_
#define	_ConnectionSetup_H_

extern "C" {
    #include <RRCConnectionSetup.h>
}

#include <stdio.h>
#include <sys/types.h>

typedef struct EncodedConnectionSetup {
    uint8_t* ptr;
    size_t size;
} EncodedConnectionSetup;

EncodedConnectionSetup CreateConnectionSetup();

RRCConnectionSetup_t* DecodeConnectionSetup(uint8_t* buffer, size_t size);

#endif