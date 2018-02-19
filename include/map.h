#ifndef MAP_H
#define MAP_H

struct sMap
{
	struct sMap** subMaps;
	void* data;
	int size;
}
typedef struct sMap Map;

#endif