#include <stdlib.h>
#include "writebuffer.h"

#ifdef __unix__
	#include <unistd.h>
	#include <sys/stat.h>
	#include <sys/types.h>
#else
	#include <sys\stat.h>
#endif

WriteBuffer* createWriteBuffer(const char* outFile, int bufferSize)
{
	WriteBuffer* writeBuffer = malloc(sizeof(WriteBuffer));
	writeBuffer.bufferSize = bufferSize;
	writeBuffer.buffer = malloc(sizeof(uint8_t)*bufferSize);
	writeBuffer.pos = 0;
	//TODO mkdir
	writeBuffer.fOut = fopen(outFile, "wb");
	return writeBuffer;
}
void freeWriteBuffer(WriteBuffer* writeBuffer)
{
	if (writeBuffer.fOut != NULL)
	{
		fclose(writeBuffer.fOut);
		writeBuffer.fOut = NULL;
	}
	if (writeBuffer.buffer != NULL)
	{
		free(writeBuffer.buffer);
		writeBuffer.buffer = NULL;
	}
	free(writeBuffer);
	writeBuffer = NULL;
}
void seekWriteBuffer(WriteBuffer* writeBuffer, int pos)
{
	fwrite(writeBuffer.buffer, sizeof(uint8_t), writeBuffer.pos, writeBuffer.fOut);//TODO return
	//TODO fseek
	writeBuffer.pos = 0;
}
void writeWriteBuffer(WriteBuffer* writeBuffer, uint8_t* data, int length)
{
	if (writeBuffer.pos+length > writeBuffer.bufferSize)
	{
		for (int i=0; i<length; i++)
		{
			if (writeBuffer.pos >= writeBuffer.bufferSize)
			{
				fwrite(writeBuffer.buffer, sizeof(uint8_t), writeBuffer.pos, writeBuffer.fOut);//TODO return
				writeBuffer.pos = 0;
			}
			writeBuffer.buffer[writeBuffer.pos] = data[i];
			writeBuffer.pos++;
		}
	}
	else
	{
		for (int i=0; i<length; i++)
		{
			writeBuffer.buffer[writeBuffer.pos+i] = data[i];
		}
		writeBuffer.pos += length;
	}
}
uint8_t* closeWriteBuffer(WriteBuffer* writeBuffer)
{
	if (writeBuffer.fOut != NULL)
	{
		fclose(writeBuffer.fOut);
		writeBuffer.fOut = NULL;
	}
}