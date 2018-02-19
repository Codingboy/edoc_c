#include <stdlib.h>
#include "readbuffer.h"

#ifdef __unix__
	#include <unistd.h>
	#include <sys/stat.h>
	#include <sys/types.h>
#else
	#include <sys\stat.h>
#endif

ReadBuffer* createReadBuffer(const char* inFile, int bufferSize)
{
	ReadBuffer* readBuffer = malloc(sizeof(ReadBuffer));
	readBuffer.bufferSize = bufferSize;
	readBuffer.bufferPos = 0;
	readBuffer.pos = 0;
	struct stat fileStat;
	stat(inFile, &fileStat);
	readBuffer.fileSize = fileStat.st_size;
	if (readBuffer.fileSize < readBuffer.bufferSize)
	{
		readBuffer.bufferSize = readBuffer.fileSize;
	}
	readBuffer.fIn = fopen(inFile, "rb");
	if (readBuffer.fIn == NULL)
	{
		//TODO
	}
	readBuffer.buffer = malloc(sizeof(uint8_t)*readBuffer.bufferSize);
	int readBytes = fread(readBuffer.buffer, sizeof(uint8_t), readBuffer.bufferSize, readBuffer.fIn);
	if (readBytes != readBuffer.bufferSize)
	{
		//TODO
	}
	return readBuffer;
}
void freeReadBuffer(ReadBuffer* readBuffer)
{
	if (readBuffer.fIn != NULL)
	{
		fclose(readBuffer.fIn);
		readBuffer.fIn = NULL;
	}
	if (readBuffer.buffer != NULL)
	{
		free(readBuffer.buffer);
		readBuffer.buffer = NULL;
	}
	free(readBuffer);
	readBuffer = NULL;
}
void seekReadBuffer(ReadBuffer* readBuffer, int pos)
{
	readBuffer.bufferPos = pos
	readBuffer.pos = pos
	self.fIn.seek(pos)//TODO fseek
	int readBytes = fread(readBuffer.buffer, sizeof(uint8_t), readBuffer.bufferSize, readBuffer.fIn);//TODO limit by filesize and current pos
	if (readBytes != readBuffer.bufferSize)
	{
		//TODO
	}
}
uint8_t* readReadBuffer(ReadBuffer* readBuffer, int length, int* returnLength)
{
	if (readBuffer.pos+length > readBuffer.fileSize)
	{
		length = readBuffer.fileSize-readBuffer.pos;
	}
	if (length == 0)
	{
		*returnLength = 0;
		return NULL;
	}
	uint8_t* returnValue = malloc(sizeof(uint8_t)*length);
	*returnLength = length;
	if (readBuffer.pos+length > readBuffer.bufferPos+readBuffer.bufferSize)
	{
		int offset = readBuffer.bufferPos+readBuffer.bufferSize-readBuffer.pos;
		for (int i=0; i<readBuffer.bufferPos+readBuffer.bufferSize-readBuffer.pos; i++)
		{
			returnValue[i] = readBuffer.buffer[i+readBuffer.pos-readBuffer.bufferPos];
		}
		seekReadBuffer(readBuffer, readBuffer.bufferPos+readBuffer.bufferSize);//TODO ensure buffer contains all data (length<=bufferSize)
		for (int i=0; i<readBuffer.pos+length-readBuffer.bufferPos-offset; i++)
		{
			returnValue[i+offset] = readBuffer.buffer[i];
		}
	}
	else
	{
		for (int i=0; i<length; i++)
		{
			returnValue[i] = readBuffer.buffer[i+readBuffer.pos-readBuffer.bufferPos];
		}
	}
	readBuffer.pos = readBuffer.pos+length;
	return returnValue;
}
uint8_t* closeReadBuffer(ReadBuffer* readBuffer)
{
	if (readBuffer.fIn != NULL)
	{
		fclose(readBuffer.fIn);
		readBuffer.fIn = NULL;
	}
}