#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "archiver.h"

#ifdef __unix__
	#include <unistd.h>
	#include <sys/stat.h>
	#include <sys/types.h>
#else
	#include <sys\stat.h>
#endif

#ifdef __unix__

#include <dirent.h>
#include <sys/types.h>

char** listDir(char* folder, int* returnLength)
{
	DIR* dir = opendir(folder);
	int returnValueSize = 16;
	int returnValueLength = 0;
	char** returnValue = malloc(sizeof(char*)*returnValueSize);
	while (1)
	{
		struct dirent* openDir = readdir(dir);
		if (openDir == NULL)
		{
			break;
		}
		int subFileSize = strlen(openDir->d_name)+1;
		char* subFile = malloc(sizeof(char)*subFileSize);
		strcpy(subFile, openDir->d_name);
		returnValueLength++;
		if (returnValueLength == returnValueSize)
		{
			returnValueSize *= 2;
			returnValue = realloc(returnValue, sizeof(char*)*returnValueSize);
		}
	}
	closedir(dir);
	returnValue = realloc(returnValue, sizeof(char*)*returnValueLength);
	*returnLength = returnValueLength;
	return returnValue;
}
int isFile(char* path)
{
	struct stat stats;
	stat(path, &stats);
	return stats.st_mode & S_IFREG;
}
int isDir(char* path)
{
	struct stat stats;
	stat(path, &stats);
	return stats.st_mode & S_IFDIR;
}

#elif __WIN32__ || _MSC_VER
#include <windows.h>
//TODO implement all linux functions
void list_dir(const char *path)
{
   WIN32_FIND_DATA dir;
   HANDLE fhandle;
   char directory[256];

   /* unsicher, besser wäre - falls vorhanden - snprintf() */
   sprintf(directory,"%s\\*.*",path);
   /* Handle auf das Verzeichnis directory */
   if ((fhandle=FindFirstFile(directory,&dir)) !=
                             INVALID_HANDLE_VALUE) {
      do {  /* Verzeichnis auslesen */
         printf("%s\n", dir.cFileName);
      } while(FindNextFile(fhandle,&dir));
   }
   FindClose(fhandle);
}
#endif

Archiver* createArchiver(char* folder, int folderLength)
{
#ifdef __unix__
	char sep = '/';
#elif __WIN32__ || _MSC_VER
	char sep = '\\';
#endif
	while (folder[folderLength-1] == sep)
	{
		folderLength--;
	}
	Archiver* archiver = malloc(sizeof(Archiver));
	archiver->readBuffer = NULL;
	archiver->files = NULL;
	archiver->filesLength = 0;
	int index = folderLength-1;
	for (; index>=0; index--)
	{
		if (folder[index] == sep)
		{
			break;
		}
	}
	if (index != -1)
	{
		index++;
		archiver->folder = malloc(sizeof(char)*index);
		for (int i=0; i<index; i++)
		{
			archiver->folder[i] = folder[i];
		}
		archiver->folderLength = index;
	}
	else
	{
		archiver->folder = NULL;
		archiver->folderLength = 0;
	}
	if (isFile(folder))
	{
		archiver->filesSize = 1;
		archiver->files = malloc(sizeof(char*)*archiver->filesSize);
		char* file = NULL;
		if (index != -1)
		{
			file = malloc(sizeof(char)*(folderLength-index+1));
			for (int i=0; i<folderLength-index; i++)
			{
				file[i] = folder[index+i];
			}
			file[folderLength-index] = '\0';
		}
		else
		{
			file = malloc(sizeof(char)*(folderLength+1));
			for (int i=0; i<folderLength; i++)
			{
				file[i] = folder[i];
			}
			file[folderLength] = '\0';
		}
		archiver->files[0] = file;
		archiver->filesLength = 1;
		if (archiver->filesLength == archiver->filesSize)
		{
			archiver->filesSize *= 2;
			archiver->files = realloc(archiver->files, sizeof(char*)*archiver->filesSize);
		}
	}
	else if (isDir(folder))
	{
		archiver->filesSize = 1;
		archiver->files = malloc(sizeof(char*)*archiver->filesSize);
		int filesLength;
		char** files = listDir(folder, &filesLength);
		for (int i=0; i<filesLength; i++)
		{
			char* file = files[i];
			int fileLength = strlen(file);
			int pathLength = folderLength+1+fileLength;
			char* path = malloc(sizeof(char)*(pathLength+1));
			for (int i=0; i<folderLength; i++)
			{
				path[i] = folder[i];
			}
			path[folderLength] = sep;
			for (int i=0; i<fileLength; i++)
			{
				path[folderLength+1+i] = file[i];
			}
			path[folderLength+1+fileLength] = '\0';
			int newPathLength = pathLength-archiver->folderLength;
			char* newPath = malloc(sizeof(char)*(newPathLength+1));
			for (int i=0; i<newPathLength; i++)
			{
				newPath[i] = path[archiver->folderLength+i];
			}
			newPath[newPathLength] = '\0';
			free(path);
			path = NULL;
			archiver->files[archiver->filesLength] = newPath;
			archiver->filesLength++;
			if (archiver->filesLength == archiver->filesSize)
			{
				archiver->filesSize *= 2;
				archiver->files = realloc(archiver->files, sizeof(char*)*archiver->filesSize);
			}
		}
		for (int i=0; i<filesLength; i++)
		{
			free(files[i]);
			files[i] = NULL;
		}
		free(files);
		files = NULL;
	}
	archiver->file = NULL;
	archiver->fileLength = 0;
	archiver->readSize = 1024;
	return archiver;
}
void freeArchiver(Archiver* archiver)
{
	if (archiver->readBuffer != NULL)
	{
		freeReadBuffer(archiver->readBuffer);
		archiver->readBuffer = NULL;	
	}
	if (archiver->file != NULL)
	{
		free(archiver->file);
		archiver->file = NULL;
	}
	if (archiver->folder != NULL)
	{
		free(archiver->folder);
		archiver->folder = NULL;
	}
	if (archiver->files != NULL)
	{
		for (int i=0; i<archiver->filesLength; i++)
		{
			free(archiver->files[i]);
			archiver->files[i] = NULL;
		}
		free(archiver->files);
		archiver->files = NULL;
	}
	free(archiver);
	archiver = NULL;
}
uint8_t* readArchiver(Archiver* archiver, int* returnLength)
{
#ifdef __unix__
	char sep = '/';
#elif __WIN32__ || _MSC_VER
	char sep = '\\';
#endif
	if (archiver->readBuffer == NULL)
	{
		int returnValueSize = 1024*2;//TODO ensure big enough
		int returnValueLength = 0;
		uint8_t* returnValue = malloc(sizeof(uint8_t)*returnValueSize);
		while (1)
		{
			if (archiver->filesLength == 0)
			{
				break;
			}
			else
			{
				char* f = archiver->files[archiver->filesLength-1];
printf("%s\n", f);
				int fLength = strlen(f);
				archiver->filesLength--;
				int fileLength = archiver->folderLength+fLength;
				char* file = malloc(sizeof(char)*(fileLength+1));
				for (int i=0; i<archiver->folderLength; i++)
				{
					file[i] = archiver->folder[i];
				}
				for (int i=0; i<fLength; i++)
				{
					file[archiver->folderLength+i] = f[i];
				}
				free(f);
				f = NULL;
				file[archiver->folderLength+fLength] = '\0';
				if (isDir(file))
				{
					int folderLength = fileLength-archiver->folderLength;
					char folder[folderLength+1];
					returnValue[returnValueLength] = folderLength >> 8;
					returnValue[returnValueLength+1] = folderLength & 255;
					returnValueLength += 2;
					for (int i=0; i<folderLength; i++)
					{
						returnValue[returnValueLength+i] = folder[i];
					}
					returnValueLength += folderLength;
					int fileSize = 0;
					for (int i=0; i<8; i++)
					{
						returnValue[returnValueLength+i] = (fileSize >> (8*(8-1-i))) & 255;
					}
					returnValueLength += 8;
					int filesLength;
					char** files = listDir(file, &filesLength);
					for (int j=0; j<filesLength; j++)
					{
						char* subFile = files[j];
						int subFileLength = strlen(subFile);
						char* path = malloc(sizeof(char)*(1+subFileLength+1));
						path[0] = sep;
						for (int i=0; i<subFileLength; i++)
						{
							path[1+i] = subFile[i];
						}
						path[1+subFileLength] = '\0';
						archiver->files[archiver->filesLength] = path;
						archiver->filesLength++;
						if (archiver->filesLength == archiver->filesSize)
						{
							archiver->filesSize *= 2;
							archiver->files = realloc(archiver->files, sizeof(char)*archiver->filesSize);
						}
						free(files[j]);
						files[j] = NULL;
						subFile = NULL;
					}
					free(files);
					files = NULL;
				}
				else if (isFile(file))
				{
					archiver->readBuffer = createReadBuffer(file, archiver->readSize);
					archiver->file = file;
					archiver->fileLength = fileLength;
					returnValue[returnValueLength] = fileLength >> 8;
					returnValue[returnValueLength+1] = fileLength & 255;
					returnValueLength += 2;
					for (int i=0; i<fileLength; i++)
					{
						returnValue[returnValueLength+i] = file[i];
					}
					returnValueLength += fileLength;
					struct stat fileStat;
					stat(file, &fileStat);
					int fileSize = fileStat.st_size;
					for (int i=0; i<8; i++)
					{
						returnValue[returnValueLength+i] = (fileSize >> (8*(8-1-i))) & 255;
					}
					returnValueLength += 8;
					break;
				}
			}
		}
		*returnLength = returnValueLength;
		return returnValue;
	}
	else
	{
		int dataLength;
		uint8_t* data = readReadBuffer(archiver->readBuffer, archiver->readSize, &dataLength);
		if (dataLength == 0)
		{
			freeReadBuffer(archiver->readBuffer);
			archiver->readBuffer = NULL;
			//TODO delete file
					/*try:
						os.remove(self.file)
					except PermissionError as e:
						getLog().error("could not remove file: "+self.file, e)
					index = self.file.rfind(os.sep)
					if index != -1:
						folder = self.file[:index]
						files = os.listdir(folder)
						if len(files) == 0:
							shutil.rmtree(folder)*/
			free(data);
			data = NULL;
			return readArchiver(archiver, returnLength);
		}
printf("\n\n%i\n", dataLength);
for (int i=0; i<dataLength; i++)
{
	printf("%c", data[i]);
}
		*returnLength = dataLength;
		return data;
	}
}
