#ifndef DECOMPRESSOR_H
#define DECOMPRESSOR_H

struct sDecompressorContainer
{
	int prev;
	uint8_t* data;
	int dataLength;
}
typedef struct sDecompressorContainer DecompressorContainer;

struct sDecompressor
{
	DecompressorContainer** map;
	int size;
	int maxSize;
	uint8_t* buffer;
	int bufferLength;
}
typedef struct sDecompressor Decompressor;

#endif