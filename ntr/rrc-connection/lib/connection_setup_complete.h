#ifndef	_ConnectionSetupComplete_H_
#define	_ConnectionSetupComplete_H_

#include <RRCConnectionSetupComplete.h>
#include <stdio.h>
#include <sys/types.h>

typedef struct EncodedConnectionSetupComplete {
    uint8_t* ptr;
    size_t size;
} EncodedConnectionSetupComplete;

EncodedConnectionSetupComplete CreateConnectionSetupComplete();

RRCConnectionSetupComplete_t* DecodeConnectionSetupComplete(uint8_t* buffer, size_t size);

#endif