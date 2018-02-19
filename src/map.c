#include <stdlib.h>
#include "map.h"

Map* createMap(int size)
{
	Map* map = malloc(sizeof(Map));
	map.size = size;
	map.subMaps = malloc(sizeof(Map*)*size);
	for (int i=0; i<size; i++)
	{
		map.subMaps[i] = NULL;
	}
	map.data = NULL;
}
void freeMap(Map* map)
{
	if (map.subMaps != NULL)
	{
		for (int i=0; i<map.size; i++)
		{
			Map* subMap = map.subMaps[i];
			if (subMap != NULL)
			{
				freeMap(subMap);
			}
			map.subMaps[i] = NULL;
		}
		free(map.subMaps);
		map.subMaps = NULL;
	}
	free(map);
	map = NULL;
}
void setDataMap(Map* map, void* data)
{
	map.data = data;
}
int containsMap(Map* map, int* keys, int keysLength)
{
	Map* m = map;
	for (int i=0; i<keysLength; i++)
	{
		int key = keys[i];
		Map* subMap = m.subMaps[key];
		if (subMap != NULL)
		{
			m = subMap;
		}
		else
		{
			return 0;
		}
	}
	return 1;
}
void setMap(Map* map, int key, Map* newMap)
{
	map.subMaps[key] = newMap;
}