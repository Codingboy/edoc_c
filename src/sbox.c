#include <stdlib.h>
#include "sbox.h"

SBox* createSBox(uint8_t* pw)
{
	SBox* sBox = malloc(sizeof(SBox));
	sBox.encodeMap = malloc(sizeof(uint8_t)*256);
	sBox.decodeMap = malloc(sizeof(uint8_t)*256);
	uint8_t state[256];
	for (int i=0; i<256; i++)
	{
		state[i] = 0;
	}
	int index = 0;
	for (int i=0; i<256; i++)
	{
		int emptyCounter = 0;
		int maxEmpty = 256-i;
		int targetEmpty = 1+(pw[i]%maxEmpty);
		while (emptyCounter < targetEmpty)
		{
			if (state[index] == 0)
			{
				emptyCounter++;
			}
			if (emptyCounter < targetEmpty)
			{
				index = (index+1)%256;
			}
		}
		sBox.encodeMap[index] = i;
		state[index] = 1;
	}
	for (int i=0; i<256; i++)
	{
		sBox.decodeMap[sBox.encodeMap[i]] = i
	}
}
void freeSBox(SBox* sBox)
{
	free(sBox.encodeMap);
	sBox.encodeMap = NULL;
	free(sBox.decodeMap);
	sBox.decodeMap = NULL;
	free(sBox);
	sBox = NULL;
}
uint8_t encodeSBox(SBox* sBox, uint8_t plain)
{
	return sBox.encodeMap[plain];
}
uint8_t decodeSBox(SBox* sBox, uint8_t encoded)
{
	return sBox.decodeMap[encoded];
}