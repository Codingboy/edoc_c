
#include <stdio.h>
#include <stdlib.h>

#include "archiver.h"

#ifdef __unix__
#include <dirent.h>
#include <sys/types.h>
void list_dir(const char *path)
{
   DIR *dirptr;
   struct dirent *dir;

   if ((dirptr=opendir(path)) == NULL)
      return;
   while((dir=readdir(dirptr)) != NULL)
      printf("%s\n",dir->d_name);
   closedir(dirptr);
}

#elif __WIN32__ || _MSC_VER
#include <windows.h>
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
	char sep = "/";
#elif __WIN32__ || _MSC_VER
	char sep = "\\";
#endif
	while (folder[folderLength-1] == sep)
	{
		folderLength--;
	}
	Archiver* archiver = malloc(sizeof(Archiver));
	archiver.readBuffer = NULL;
	archiver.files = NULL;
	archiver.filesLength = 0;
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
		archiver.folder = malloc(sizeof(char)*index);
		for (int i=0; i<index; i++)
		{
			archiver.folder[i] = folder[i];
		}
		archiver.folderLength = index;
	}
	else
	{
		archiver.folder = NULL;
		archiver.folderLength = 0;
	}
	if (isFile(folder))//TODO isfile
	{
		archiver.files = malloc(sizeof(char*)*1);
		if (index != -1)
		{
			char* file = malloc(sizeof(char)*(folderLength-index+1));
			for (int i=0; i<folderLength-index; i++)
			{
				file[i] = folder[index+i];
			}
			file[folderLength-index] = '\0';
		}
		else
		{
			char* file = malloc(sizeof(char)*(folderLength+1));
			for (int i=0; i<folderLength; i++)
			{
				file[i] = folder[i];
			}
			file[folderLength] = '\0';
		}
		archiver.files[0] = file;
		archiver.filesLength = 1;
	}
	else if (isDir(folder))//TODO isdir
	{
		int filesLength;
		char** files = listDir(folder, folderLength, &filesLength);//TODO listDir
		for (int i=0; i<filesLength; i++)
		{
			char* file = files[i];
			int fileLength;//TODO strlen(file)
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
			int newPathLength = pathLength-archiver.folderLength;
			char* newPath = malloc(sizeof(char)*(newPathLength+1));
			for (int i=0; i<newPathLength; i++)
			{
				newPath[i] = path[archiver.folderLength+i];
			}
			newPath[newPathLength] = '\0';
			free(path);
			path = NULL;
			//TODO add newPath to archiver.files
		}
		for (int i=0; i<filesLength; i++)
		{
			free(files[i]);
			files[i] = NULL;
		}
		free(files);
		files = NULL;
	}
	archiver.file = NULL;
	archiver.fileLength = 0;
	archiver.readSize = 1024;
}

uint8_t* readArchiver(Archiver* archiver, int* returnLength)
{
	int returnValueSize = 1024;//TODO ensure big enough
	int returnValueLength = 0;
	uint8_t* returnValue = malloc(sizeof(uint8_t)*returnValueSize);
	if (archiver.buffer == NULL)
	{
		while (1)
		{
			if (archiver.filesLength == 0)
			{
				break;
			}
			else
			{
				char f = archiver.files[archiver.filesLength];
				int fLength = strlen(f);
				archiver.filesLength--;
				int fileLength = archiver.folderLength+fLength;
				char file[fileLength+1];
				for (int i=0; i<archiver.folderLength; i++)
				{
					file[i] = archiver.folder[i];
				}
				for (int i=0; i<fLength; i++)
				{
					file[archiver.folderLength+i] = f[i];
				}
				file[archiver.folderLength+fLength] = '\0';
				if (isDir(file))
				{
					int folderLength = fileLength-archiver.folderLength;
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
						char* path = malloc(sizeof(char)*(1+subFileLength+1)];
						path[0] = sep;
						for (int i=0; i<subFileLength; i++)
						{
							path[1+i] = subFile[i];
						}
						path[1+subFileLength] = '\0';
						archiver.files[archiver.filesLength] = path;
						archiver.filesLength++;
						if (archiver.filesLength == archiver.filesSize)
						{
							archiver.filesSize *= 2;
							archiver.files = realloc(archiver.filesSize);//TODO
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
					archiver.readBuffer = createReadBuffer(file);//TODO
					archiver.file = file;
					archiver.fileLength = fileLength;
					returnValue[returnValueLength] = fileLength >> 8;
					returnValue[returnValueLength+1] = fileLength & 255;
					returnValueLength += 2;
					for (int i=0; i<fileLength; i++)
					{
						returnValue[returnValueLength+i] = file[i];
					}
					returnValueLength += fileLength;
					int fileSize = 0;//TODO
					for (int i=0; i<8; i++)
					{
						returnValue[returnValueLength+i] = (fileSize >> (8*(8-1-i))) & 255;
					}
					returnValueLength += 8;
					break;
				}
			}
		}
	}
	else
	{
		//TODO implement
			ba = self.readBuffer.read(self.readSize)
			if len(ba) == 0:
				self.readBuffer.close()
				self.readBuffer = None
				if self.delete:
					try:
						os.remove(self.file)
					except PermissionError as e:
						getLog().error("could not remove file: "+self.file, e)
					index = self.file.rfind(os.sep)
					if index != -1:
						folder = self.file[:index]
						files = os.listdir(folder)
						if len(files) == 0:
							shutil.rmtree(folder)
				ba = self.read()
	}
}