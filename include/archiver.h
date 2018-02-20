#ifndef ARCHIVER_H
#define ARCHIVER_H

#include "readbuffer.h"

struct sArchiver
{
	ReadBuffer* readBuffer;
	char** files;
	int filesLength;
	int filesSize;
	char* folder;
	int folderLength;
	char* file;
	int fileLength;
	int readSize;
};
typedef struct sArchiver Archiver;

char** listDir(char* folder, int* returnLength);
int isFile(char* path);
int isDir(char* path);
Archiver* createArchiver(char* folder, int folderLength);
uint8_t* readArchiver(Archiver* archiver, int* returnLength);

#endif
