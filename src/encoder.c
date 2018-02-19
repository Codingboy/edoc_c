#include <stdlib.h>
#include "encoder.h"

Encoder* createEncoder(char* pw, int pwLength)
{
	Encoder* encoder = malloc(sizeof(Encoder));
	uint8_t password[4096];
	int index = 0;
	while (index < 4096)
	{
		password[index] = pw[index%pwLength];
		index++;
	}
	encoder.spBox = createSPBox(password);
	encoder.buffer = NULL;
	encoder.bufferLength = 0;
	encoder.seeded = 0;
}
void freeEncoder(Encoder* encoder)
{
	freeSPBox(encoder.spBox);
	encoder.spBox = NULL;
	if (encoder.buffer != NULL)
	{
		free(encoder.buffer);
		encoder.buffer = NULL;
	}
	free(encoder);
	encoder = NULL;
}
uint8_t* encodeEncoder(Encoder* encoder, char* plain, int length, int* returnLength)
{
	int returnValueLength = ((encoder.bufferLength+length)/256)*257;
	if (!encoder.seeded)
	{
		returnValueLength += 256;
	}
	uint8_t* returnValue = malloc(sizeof(uint8_t)*returnValueLength);
	int returnValueIndex = 0;
	if (!encoder.seeded)
	{
		uint8_t* seed = getSeedSBPox(encoder.spBox);
		for (int i=0; i<256; i++)
		{
			returnValue[i] = seed[i];
		}
		returnValueIndex += 256;
		free(seed);
		seed = NULL;
		encoder.seeded = 1;
	}
	int processedBytes = 0;
	uint8_t data[encoder.bufferLength+length];
	for (int i=0; i<encoder.bufferLength; i++)
	{
		data[i] = encoder.buffer[i];
	}
	for (int i=0; i<length; i++)
	{
		data[encoder.bufferLength+i] = plain[i];
	}
	int bufferLength = encoder.bufferLength;
	if (encoder.buffer != NULL)
	{
		free(encoder.buffer);
		encoder.buffer = NULL;
		encoder.bufferLength = 0;
	}
	while ((bufferLength+length-processedBytes)/256 > 0)
	{
		uint8_t* array = &(data[processedBytes]);//TODO does it work?
		uint8_t* encoded = encodeSPBox(encoder.spBox, array);
		for (int i=0; i<256; i++)
		{
			returnValue[returnValueIndex+i] = encoded[i];
		}
		free(encoded);
		encoded = NULL;
		if (plain != NULL)
		{
			returnValue[returnValueIndex+256] = 255;
		}
		else
		{
			returnValue[returnValueIndex+256] = realLength-1;
		}
		returnValueIndex += 257;
		processedBytes += 256;
	}
	int encoder.bufferLength = length%256;
	if (encoder.bufferLength != 0)
	{
		encoder.buffer = malloc(sizeof(uint8_t)*encoder.bufferLength);
		for (int i=0; i<encoder.bufferLength; i++)
		{
			encoder.buffer[i] = data[((encoder.bufferLength+length)/256)*256 + i];//TODO ((encoder.bufferLength+length)/256)*256 == processedBytes
		}
	}
	*returnLength = returnValueLength;
	return returnValue;
}
uint8_t* closeEncoder(Encoder* encoder, int* returnLength)
{
	int bufferLength = encoder.bufferLength;
	if (bufferLength == 0)
	{
		*returnLength = 0;
		return NULL;
	}
	if (bufferLength < 256)
	{
		encoder.buffer = realloc(typeof(uint8_t)*256);
		for (int i=bufferLength; i<256; i++)
		{
			encoder.buffer[i] = rand()%256;
		}
		encoder.bufferLength = 256;
	}
	return encodeEncoder(encoder, NULL, bufferLength, returnLength);
}