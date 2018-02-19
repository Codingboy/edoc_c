#include <stdlib.h>
#include "decoder.h"

Decoder* createDecoder(char* pw, int pwLength)
{
	Decoder* decoder = malloc(sizeof(Decoder));
	uint8_t password[4096];
	int index = 0;
	while (index < 4096)
	{
		password[index] = pw[index%pwLength];
		index++;
	}
	decoder.spBox = createSPBox(password);
	decoder.buffer = NULL;
	decoder.bufferLength = 0;
	decoder.seeded = 0;
}
void freeDecoder(Decoder* decoder)
{
	freeSPBox(decoder.spBox);
	decoder.spBox = NULL;
	if (decoder.buffer != NULL)
	{
		free(decoder.buffer);
		decoder.buffer = NULL;
	}
	free(decoder);
	decoder = NULL;
}
uint8_t* decodeDecoder(Decoder* decoder, uint8_t* encoded, int length, int* returnLength)
{
	int realLength = length;
	if (encoded == NULL && length > 0)
	{
		length = 0;
	}
	int returnValueLength = ((decoder.bufferLength+length)/257)*256;
	int seeded = 0;
	if (!decoder.seeded)
	{
		if (decoder.bufferLength+length >= 256)
		{
			returnValueLength -= 256;
			uint8_t seed[256];
			for (int i=0; i<256; i++)
			{
				if (i < decoder.bufferLength)
				{
					seed[i] = decoder.buffer[i];
				}
				else
				{
					seed[i] = encoded[i-decoder.bufferLength];
				}
			}
			setSeedSPBox(decoder.spBox, &seed);
			decoder.seeded = 1;
			seeded = 1;
		}
	}
	uint8_t* returnValue = malloc(sizeof(uint8_t)*returnValueLength);
	int returnValueIndex = 0;
	int processedBytes = 0;
	int dataLength = decoder.bufferLength+length;
	uint8_t data[decoder.bufferLength+length];
	if (seeded)
	{
		dataLength -= 256;
		if (decoder.bufferLength < 256)
		{
			for (int i=256-decoder.bufferLength; i<length; i++)
			{
				data[i-(256-decoder.bufferLength)] = encoded[i];
			}
		}
		else
		{
			for (int i=256; i<decoder.bufferLength; i++)
			{
				data[i-256] = decoder.buffer[i];
			}
			for (int i=0; i<length; i++)
			{
				data[decoder.bufferLength+i] = encoded[i];
			}
		}
	}
	else
	{
		for (int i=0; i<decoder.bufferLength; i++)
		{
			data[i] = decoder.buffer[i];
		}
		for (int i=0; i<length; i++)
		{
			data[decoder.bufferLength+i] = encoded[i];
		}
	}
	int bufferLength = decoder.bufferLength;
	if (decoder.buffer != NULL)
	{
		free(decoder.buffer);
		decoder.buffer = NULL;
		decoder.bufferLength = 0;
	}
	while ((dataLength-processedBytes)/257 > 0)
	{
		uint8_t* array = &(data[processedBytes]);//TODO does it work?
		int length = data[processedBytes+256]+1;
		uint8_t* decoded = decodeSPBox(decoder.spBox, array);
		for (int i=0; i<length; i++)
		{
			returnValue[i+returnValueIndex] = decoded[i];
		}
		if (length < 256)
		{
			returnValueLength -= (256-length);
		}
		free(decoded);
		decoded = NULL;
		returnValueIndex += 256;
		processedBytes += 257;
	}
	int decoder.bufferLength = dataLength%257;
	if (decoder.bufferLength != 0)
	{
		decoder.buffer = malloc(sizeof(uint8_t)*decoder.bufferLength);
		for (int i=0; i<decoder.bufferLength; i++)
		{
			decoder.buffer[i] = data[processedBytes + i];
		}
	}
	*returnLength = returnValueLength;
	return returnValue
}
uint8_t* closeDecoder(Decoder* decoder, int* returnLength)
{
	int bufferLength = decoder.bufferLength;
	if (bufferLength == 0)
	{
		*returnLength = 0;
		return NULL;
	}
	if (bufferLength < 256)
	{
		decoder.buffer = realloc(typeof(uint8_t)*256);
		for (int i=bufferLength; i<256; i++)
		{
			decoder.buffer[i] = rand()%256;
		}
		decoder.bufferLength = 256;
	}
	return decodeDecoder(decoder, NULL, bufferLength, returnLength);
}