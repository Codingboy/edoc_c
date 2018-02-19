#ifndef READBUFFER_H
#define READBUFFER_H

#include <inttypes.h>
#include <stdio.h>

struct sReadBuffer
{
	FILE* fIn;
	int bufferSize;
	uint8_t* buffer;
	int bufferPos;
	int pos;
	int fileSize;
}
typedef struct sReadBuffer ReadBuffer;

ReadBuffer* createReadBuffer(const char* inFile, int bufferSize);
void freeReadBuffer(ReadBuffer* readBuffer);
void seekReadBuffer(ReadBuffer* readBuffer, int pos);
uint8_t* readReadBuffer(ReadBuffer* readBuffer, int length, int* returnLength);
uint8_t* closeReadBuffer(ReadBuffer* readBuffer);

#endif