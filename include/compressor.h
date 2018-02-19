#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "map.h"

struct sCompressor
{
	Map* map;
	int size;
	int maxSize;
	uint8_t* buffer;
	int bufferLength;
}
typedef struct sCompressor Compressor;

#endif