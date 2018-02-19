#ifndef SBOX_H
#define SBOX_H

#include <inttypes.h>

struct sSBox
{
	uint8_t* encodeMap;
	uint8_t* decodeMap;
}
typedef struct sSBox SBox;

SBox* createSBox(uint8_t* pw);
void freeSBox(SBox* sBox);
uint8_t encodeSBox(SBox* sBox, uint8_t plain);
uint8_t decodeSBox(SBox* sBox, uint8_t encoded);

#endif