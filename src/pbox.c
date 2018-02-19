#include <stdlib.h>
#include "pbox.h"

PBox* createPBox(uint8_t* pw)
{
	PBox* pBox = malloc(sizeof(PBox));
	pBox.encodeMap = malloc(sizeof(uint16_t)*2048);
	pBox.decodeMap = malloc(sizeof(uint16_t)*2048);
	int state[2048];
	for (int i=0; i<2048; i++)
	{
		state[i] = 0;
	}
	int index = 0;
	for (int i=0; i<2048; i++)
	{
		int emptyCounter = 0;
		int maxEmpty = 2048-i;
		int targetEmpty = 1+(pw[i]%maxEmpty);
		while (emptyCounter < targetEmpty)
		{
			if (state[index] == 0)
			{
				emptyCounter++;
			}
			if (emptyCounter < targetEmpty)
			{
				index = (index+1)%2048;
			}
		}
		pBox.encodeMap[index] = i;
		state[index] = 1;
	}
	for (int i=0; i<2048; i++)
	{
		pBox.decodeMap[pBox.encodeMap[i]] = i
	}
}
void freePBox(PBox* pBox)
{
	free(pBox.encodeMap);
	pBox.encodeMap = NULL;
	free(pBox.decodeMap);
	pBox.decodeMap = NULL;
	free(pBox);
	pBox = NULL;
}
uint8_t* encodePBox(PBox* pBox, uint8_t* plain, uint8_t seed)
{
	uint8_t* encoded = calloc(256, sizeof(uint8_t));
	for (int i=0; i<256; i++)
	{
		int indexVar = i*8+seed;
		for (int b=0; b<8; b++)
		{
			if (plain[i] & (1<<b))
			{
				int index = pBox.encodeMap[(b+indexVar)%2048]
				int index8 = index/8;
				encoded[index8] = encoded[index8]+(1<<(index%8));
			}
		}
	}
	return encoded;
}
uint8_t* decodePBox(PBox* pBox, uint8_t* encoded, uint8_t seed)
{
	uint8_t* decoded = calloc(256, sizeof(uint8_t));
	for (int i=0; i<256; i++)
	{
		int indexVar = i*8;
		for (int b=0; b<8; b++)
		{
			if (encoded[i] & (1<<b))
			{
				int index = pBox.decodeMap[indexVar+b]-seed;
				if (index < 0)
				{
					index += 2048;
				}
				int index8 = index/8;
				decoded[index8] = decoded[index8] + (1<<(index%8));
			}
		}
	}
	return decoded;
}