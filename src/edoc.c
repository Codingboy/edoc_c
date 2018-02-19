#include "version.h"
#include <stdio.h>
#include <string.h>
#include <alloca.h>

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
		//Archiver* archiver = createArchiver();//TODO
		int fileLength = strlen(file);
		int outFileLength = fileLength+5;
		char outFile[outFileLength+1];
		strncat(outFile, file, fileLength);
		strncat(outFile, ".edoc", 5);
printf("%s\n", outFile);
	}
/*
				archiver = Archiver(file, True)
				compressor = Compressor()
				encoder = Encoder(password)
				writebuffer = WriteBuffer(file+".edoc")
				while True:
					printProgress()
					breakcondition = False
					data = archiver.read()
					datalen = len(data)
					progress += datalen
					if datalen == 0:
						breakcondition = True
					data = compressor.compress(data)
*/	
	return 0;
}
