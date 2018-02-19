#include "dedecompressor.h"

DedecompressorContainer* createDedecompressorContainer(int prev, uint8_t* data, int dataLength)
{
	DedecompressorContainer* decompressorContainer = malloc(sizeof(DedecompressorContainer));
	decompressorContainer.prev = prev;
	decompressorContainer.data = data;
	decompressorContainer.dataLength = dataLength;
}
void freeDedecompressorContainer(DedecompressorContainer* decompressorContainer)
{
	if (decompressorContainer != NULL)
	{
		if (decompressorContainer.data != NULL)
		{
			free(decompressorContainer.data);
			decompressorContainer.data = NULL;
		}
		free(decompressorContainer);
		decompressorContainer = NULL;
	}
}

Dedecompressor* createDedecompressor()
{
	Dedecompressor* decompressor = malloc(sizeof(Dedecompressor));
	decompressor.size = 256;
	decompressor.maxSize = 256*256;
	decompressor.buffer = NONE;
	decompressor.bufferLength = 0;
	decompressor.map = malloc(sizeof(DedecompressorContainer)*decompressor.maxSize);
	for (int i=0; i<decompressor.maxSize; i++)
	{
		decompressor.map[i] = NULL;
	}
	for (int i=0; i<decompressor.size; i++)
	{
		int newDataLength = 1;
		uint8_t* newData = malloc(sizeof(uint8_t)*newDataLength);
		newData[0] = i;
		DecompressorContainer* decompressorContainer = createDedecompressorContainer(-1, newData, newDataLength);
		decompressor.map[i] = decompressorContainer;
	}
}
void freeDedecompressor(Dedecompressor* decompressor)
{
	if (decompressor != NULL)
	{
		if (decompressor.buffer != NULL)
		{
			free(decompressor.buffer);
			decompressor.buffer = NULL;
		}
		if (decompressor.map != NULL)
		{
			for (int i=0; i<decompressor.maxSize; i++)
			{
				if (decompressor.map[i] != NULL)
				{
					freeDedecompressorContainer(decompressor.map[i]);
					decompressor.map[i] = NULL;
				}
			}
			free(decompressor.map);
			decompressor.map = NULL;
		}
		free(decompressor);
		decompressor = NULL;
	}
}
uint8_t* decompressDedecompressor(Dedecompressor* decompressor, uint8_t* data, int dataLength, int* returnLength)
{
	int returnValueLength = 0;
	int returnValueSize = dataLength*16;
	uint8_t* returnValue = malloc(sizeof(uint8_t)*returnValueSize);
	int byteMessageSize = dataLength+decompressor.bufferLength;
	uint8_t* byteMessage[byteMessageSize];
	int byteMessageIndex = 0;
	for (int i=0; i<decompressor.bufferLength; i++)
	{
		byteMessage[i] = decompressor.buffer[i];
	}
	for (int i=0; i<dataLength; i++)
	{
		byteMessage[decompressor.bufferLength+i] = data[i];
	}
	if (decompressor.buffer != NULL)
	{
		free(decompressor.buffer);
		decompressor.buffer = NULL;
		decompressor.bufferLength = 0;
	}
	while (1)
	{
		int requiredLength = 3;
		if (decompressor.size == decompressor.maxSize)
		{
			requiredLength = 2;
		}
		if (byteMessageSize - byteMessageIndex >= requiredLength)
		{
			if (decompressor.size == decompressor.maxSize)
			{
				int index = 0;
				index += (byteMessage[byteMessageIndex]) << 8;
				index += byteMessage[byteMessageIndex+1];
				byteMessageIndex += 2;
				uint8_t* tmp = decompressor.map[index].data;
				int tmpLength = decompressor.map[index].dataLength;
				for (int i=0; i<tmpLength; i++)
				{
					returnValue[returnValueIndex+i] = tmp[i];
				}
				returnValueIndex += tmpLength;
			}
			else
			{
				int prev = 0;
				prev += (byteMessage[byteMessageIndex]) << 8;
				prev += byteMessage[byteMessageIndex+1];
				int newDataLength = decompressor.map[prev].dataLength+1;
				uint8_t* newData = malloc(sizeof(uint8_t)*newDataLength);
				for (int i=0; i<newDataLength-1; i++)
				{
					newData[i] = decompressor.map[prev].data[i];
				}
				newData[newDataLength-1] = byteMessage[byteMessageIndex+2];
				byteMessageIndex += 3;
				DecompressorContainer* decompressorContainer = createDedecompressorContainer(prev, newData, newDataLength);
				decompressor.map[decompressor.size] = decompressorContainer;
				decompressor.size++;
				for (int i=0; i<newDataLength; i++)
				{
					returnValue[returnValueIndex+i] = newData[i];
				}
				returnValueIndex += newDataLength;
			}
		}
		else
		{
			decompressor.bufferLength = byteMessageSize-byteMessageIndex;
			decompressor.buffer = malloc(sizeof(uint8_t)*decompressor.bufferLength);
			for (int i=0; byteMessageIndex+i<byteMessageSize; i++)
			{
				decompressor.buffer[i] = byteMessage[byteMessageIndex+i];
			}
		}
	}
	*returnLength = returnValueIndex;
	return returnValue;
}

uint8_t* closeDecompressor(Dedecompressor* decompressor)
{
	//TODO
	return NULL;
}
