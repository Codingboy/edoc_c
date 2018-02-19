#ifndef ARCHIVER_H
#define ARCHIVER_H

#include "map.h"

struct sArchiver
{
	Map* map;
	int size;
	int maxSize;
	uint8_t* buffer;
	int bufferLength;
}
typedef struct sArchiver Archiver;

#endif