#ifndef SPBOX_H
#define SPBOX_H

#include <inttypes.h>
#include "sbox.h"
#include "pbox.h"

struct sSPBox
{
	SBox** sBoxes;
	uint8_t* seed;
	PBox* pBox;
}
typedef struct sSPBox SPBox;

SPBox* createSPBox(uint8_t* pw);
void freeSPBox(SPBox* spBox);
uint8_t* encodeRoundSPBox(SPBox* spBox, uint8_t* plain, int round, uint8_t pSeed);
uint8_t* decodeRoundSPBox(SPBox* spBox, uint8_t* encoded, int round, uint8_t pSeed);
uint8_t* encodeSPBox(SPBox* spBox, uint8_t* plain);
uint8_t* decodeSPBox(SPBox* spBox, uint8_t* encoded);

#endif