#ifndef WRITEBUFFER_H
#define WRITEBUFFER_H

#include <inttypes.h>
#include <stdio.h>

struct sWriteBuffer
{
	FILE* fOut;
	int bufferSize;
	uint8_t* buffer;
	int pos;
};
typedef struct sWriteBuffer WriteBuffer;

WriteBuffer* createWriteBuffer(const char* outFile, int bufferSize);
void freeWriteBuffer(WriteBuffer* writeBuffer);
void seekWriteBuffer(WriteBuffer* writeBuffer, int pos);
void writeWriteBuffer(WriteBuffer* writeBuffer, uint8_t* data, int length);
void closeWriteBuffer(WriteBuffer* writeBuffer);

#endif
