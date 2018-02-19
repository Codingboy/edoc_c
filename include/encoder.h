#ifndef ENCODER_H
#define ENCODER_H

#include <inttypes.h>
#include "spbox.h"

struct sEncoder
{
	SPBox* spBox;
	uint8_t* buffer;
	int bufferLength;
	int seeded;
}
typedef struct sEncoder Encoder;

Encoder* createEncoder(char* pw, int pwLength);
void freeEncoder(Encoder* encoder);
uint8_t* encodeEncoder(Encoder* encoder, char* plain, int length, int* returnLength);
uint8_t* closeEncoder(Encoder* encoder, int* returnLength);

#endif