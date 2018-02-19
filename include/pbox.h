#ifndef PBOX_H
#define PBOX_H

#include <inttypes.h>

struct sPBox
{
	uint16_t* encodeMap;
	uint16_t* decodeMap;
}
typedef struct sPBox PBox;

PBox* createPBox(uint8_t* pw);
void freePBox(PBox* pBox);
uint8_t* encodePBox(PBox* pBox, uint8_t* plain, uint8_t seed);
uint8_t* decodePBox(PBox* pBox, uint8_t* encoded, uint8_t seed);

#endif