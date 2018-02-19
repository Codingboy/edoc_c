#ifndef DECODER_H
#define DECODER_H

#include <inttypes.h>
#include "spbox.h"

struct sDecoder
{
	SPBox* spBox;
	uint8_t* buffer;
	int bufferLength;
	int seeded;
}
typedef struct sDecoder Decoder;

Decoder* createDecoder(char* pw, int pwLength);
void freeDecoder(Decoder* decoder);
uint8_t* decodeDecoder(Decoder* decoder, uint8_t* encoded, int length, int* returnLength);
uint8_t* closeDecoder(Decoder* decoder, int* returnLength);

#endif