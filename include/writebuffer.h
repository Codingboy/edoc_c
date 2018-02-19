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
}
typedef struct sWriteBuffer WriteBuffer;

#endif