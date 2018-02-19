#include <stdlib.h>
#include "spbox.h"

SPBox* createSPBox(uint8_t* pw)
{
	SPBox* spBox = malloc(sizeof(SPBox));
	spBox.sBoxes = malloc(sizeof(SBox*)*8);
	spBox.seed = malloc(sizeof(uint8_t)*256);
	for (int i=0; i<256; i++)
	{
		spBox.seed[i] = 1+(rand()%(256-1));
	}
	for (int s=0; s<8; s++)
	{
		uint8_t spw[256];
		for (int i=0; i<256; i++)
		{
			spw[i] = pw[s*256+i];
		}
		spBox.sBoxes[s] = createSBox(spw)
	}
	uint16_t ppw[2048];
	for (int i=0; i<2048; i++)
	{
		ppw[i] = pw[2048+i];
	}
	spBox.pBox = createPBox(ppw)
	
	ppw = bytearray(2048)
	for i in range(2048):
		ppw[i] = pw[8*256+i]
	self.pBox: PBox = PBox(ppw)
}
void freeSPBox(SPBox* spBox)
{
	freePBox(spBox.pBox);
	spBox.pBox = NULL;
	free(spBox.seed);
	spBox.seed = NULL;
	for (int i=0; i<8; i++)
	{
		freeSBox(spBox.sBoxes[i]);
		spBox.sBoxes[i] = NULL;
	}
	free(spBox.sBoxes);
	spBox.sBoxes = NULL;
	free(spBox);
	spBox = NULL;
}
uint8_t* encodeRoundSPBox(SPBox* spBox, uint8_t* plain, int round, uint8_t pSeed)
{
	uint8_t encoded[256];
	for (int i=0; i<256; i++)
	{
		int seedAtI = spBox.seed[i];
		encoded[i] = plain[i] ^ spBox.sBoxes[round].encodeMap[i] ^ seedAtI;
		for (int j=0; j<8; j++)
		{
			if (seedAtI & (1<<j))
			{
				encoded[i] = spBox.sBoxes[j].encodeMap[encoded[i]];# replacement for SBox.encode() to improve performance
			}
		}
	}
	uint8_t* ret = encodePBox(spBox.pBox, &encoded, pSeed);
	return ret;
}
uint8_t* decodeRoundSPBox(SPBox* spBox, uint8_t* encoded, int round, uint8_t pSeed)
{
	uint8_t* decoded = decodePBox(spBox.pBox, encoded, pSeed);
	for (int i=0; i<256; i++)
	{
		int seedAtI = spBox.seed[i];
		for (int invertedJ=0; invertedJ<8; invertedJ++)
		{
			int j = 8-1-invertedJ;
			if (seedAtI & (1<<j))
			{
				decoded[i] = spBox.sBoxes[j].decodeMap[decoded[i]];//replacement for SBox.decode() to improve performance
			}
		}
		decoded[i] = decoded[i] ^ spBox.sBoxes[round].encodeMap[i] ^ seedAtI
	}
	return decoded
}
uint8_t* encodeSPBox(SPBox* spBox, uint8_t* plain)
{
	uint8_t pSeed = 0;
	for (int i=0; i<256; i++)
	{
		pSeed = (pSeed+spBox.seed[i])%256;
	}
	uint8_t* encoded = encodeRoundSPBox(spBox, plain, 0, pSeed);
	for (int i=0; i<8-1; i++)
	{
		uint8_t* encodedNew = encodeRoundSPBox(spBox, encoded, i+1, pSeed);
		free(encoded);
		encoded = encodedNew;
	}
	for (int i=0; i<256; i++)
	{
		spBox.seed[i] = plain[i] ^ spBox.seed[i];
		if (spBox.seed[i] == 0)
		{
			spBox.seed[i] = 1;
		}
	}
	return encoded
}
uint8_t* decodeSPBox(SPBox* spBox, uint8_t* encoded)
{
	uint8_t pSeed = 0;
	for (int i=0; i<256; i++)
	{
		pSeed = (pSeed+spBox.seed[i])%256;
	}
	uint8_t* decoded = decodeRoundSPBox(spBox, encoded, 8-1, pSeed);
	for (int invertedI=0; invertedI<8-1; i++)
	{
		int i = 6-invertedI;
		uint8_t* decodedNew = decodeRoundSPBox(spBox, decoded, i, pSeed);
		free(decoded);
		decoded = decodedNew;
	}
	for (int i=0; i<256; i++)
	{
		spBox.seed[i] = decoded[i] ^ spBox.seed[i];
		if (spBox.seed[i] == 0)
		{
			spBox.seed[i] = 1;
		}
	}
	return decoded
}