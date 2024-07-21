#ifndef __LEVEL_H__
#define __LEVEL_H__

typedef struct mapInfoTag {
	const unsigned short *mapData;
	const unsigned short *mapPalette;
	const unsigned int *mapTiles;

	unsigned int mapDataLen;
	unsigned int mapPaletteLen;
	unsigned int mapTilesLen;

	const unsigned int mapWidth;
	const unsigned int mapHeight;
} mapInfo;

void load_level(mapInfo *map, unsigned int playerStartX, unsigned int playerStartY, OBJ_ATTR *obj_buffer);

#endif
