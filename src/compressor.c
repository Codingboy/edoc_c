#include "compressor.h"

Compressor* createCompressor()
{
	Compressor* compressor = malloc(sizeof(Compressor));
	compressor.map = createMap(256);//TODO set data
	compressor.size = 256;
	compressor.maxSize = 256*256;
	compressor.buffer = NONE;
	compressor.bufferLength = 0;
}
void freeCompressor(Compressor* compressor)
{
	if (compressor != NULL)
	{
		if (compressor.buffer != NULL)
		{
			free(compressor.buffer);
			compressor.buffer = NULL;
		}
		if (compressor.map != NULL)
		{
			freeMap(compressor.map);//TODO free data
			compressor.map = NULL;
		}
		free(compressor);
		compressor = NULL;
	}
}
uint8_t* compressCompressor(Compressor* compressor, uint8_t* data, int dataLength)
{
	uint8_t* returnValue = malloc(sizeof(uint8_t)*dataLength*2);
	int returnValueIndex = 0;
	int byteMessageSize = 128;
	uint8_t* byteMessage = malloc(sizeof(uint8_t)*byteMessageSize);
	int byteMessageLength = 0;
	if (compressor.buffer != NULL)
	{
		for (int i=0; i<compressor.bufferLength; i++)
		{
			byteMessage[i] = compressor.buffer[i];
		}
		byteMessageLength = compressor.bufferLength;
		free(compressor.buffer);
		compressor.buffer = NULL;
	}
	int dataIndex = 0;
	while (1)
	{
		if (dataIndex == dataLength)
		{
			compressor.buffer = byteMessage;
			compressor.bufferLength = byteMessageLength;
			break;
		}
		byteMessage[byteMessageLength] = data[dataIndex];
		dataIndex++;
		byteMessageLength++;
		if (containsMap(compressor.map, byteMessage, byteMessageLength))
		{
			if (compressor.size == compressor.maxSize)
			{
				uint8_t lastChar = byteMessage[byteMessageLength-1];
				byteMessageLength--;
				int prev = getMap(compressor.map, byteMessage, byteMessageLength).data.;//TODO
				returnValue[returnValueIndex] = (prev>>8) & 255;
				returnValue[returnValueIndex+1] = prev & 255;
				returnValueIndex += 2;
				byteMessage[0] = lastChar;
				byteMessageLength = 1;
			}
			else
			{
				uint8_t lastChar = byteMessage[byteMessageLength-1];
				byteMessageLength--;
				int prev = getMap(compressor.map, byteMessage, byteMessageLength).data.;//TODO
				byteMessageLength++;
				Container* container = createContainer();//TODO
				container.1 = prev;
				container.2 = compressor.size;
				setMap(compressor.map, byteMessage, byteMessageLength, container);
				compressor.size++;
				returnValue[returnValueIndex] = (prev >> 8) & 255;
				returnValue[returnValueIndex+1] = prev & 255;
				returnValue[returnValueIndex+2] = lastChar;
				returnValueIndex += 3;
				byteMessageLength = 0;
			}
		}
	}
	return returnValue;
}
uint8_t* closeCompressor(Compressor* compressor)
{
	//TODO
}
