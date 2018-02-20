#include "version.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <alloca.h>

#include "writebuffer.h"
#include "archiver.h"

int main(int argc, char* argv[])
{
	int mode = -1;
	char* file = NULL;
	char* pw = NULL;
	
	for (int i=1; i<argc; i++)
	{
		if (strcmp(argv[i], "-e") == 0)
		{
			mode = 0;
		}
		else if (strcmp(argv[i], "-d") == 0)
		{
			mode = 1;
		}
		if (strcmp(argv[i], "-p") == 0 && i+1<argc)
		{
			pw = argv[i+1];
			i++;
		}
		if (strcmp(argv[i], "-f") == 0 && i+1<argc)
		{
			file = argv[i+1];
			i++;
		}
	}
	if (file == NULL)
	{
		printf("You must specify a file.\n");
		return 1;
	}
	if (mode == -1)
	{
		printf("You must specify a mode.\n");
		return 2;
	}
	if (pw == NULL)
	{
		printf("Enter password: ");
		pw = alloca(sizeof(char)*(4096+1));
		fgets(pw, 4096+1, stdin);
	}
	if (mode == 0)
	{
		int fileLength = strlen(file);
		int outFileLength = fileLength+5;
		char outFile[outFileLength+1];
		strncpy(outFile, file, fileLength+1);
		strncat(outFile, ".edoc", 5);
		Archiver* archiver = createArchiver(file, fileLength);
		WriteBuffer* writeBuffer = createWriteBuffer(outFile, 1024);
		while (1)
		{
			int breakCondition = 0;
			int archivedDataLength = 0;
			uint8_t* archivedData = readArchiver(archiver, &archivedDataLength);
printf("\n\n%i\n", archivedDataLength);
for (int i=0; i<archivedData; i++)
{
	printf("%c", archivedData[i]);
}
			if (archivedDataLength == 0)
			{
				free(archivedData);
				archivedData = NULL;
				break;
			}
			writeWriteBuffer(writeBuffer, archivedData, archivedDataLength);
			free(archivedData);
			archivedData = NULL;
		}
		freeArchiver(archiver);
		archiver = NULL;
		freeWriteBuffer(writeBuffer);
		writeBuffer = NULL;
	}
	else
	{
		
	}
	return 0;
}
