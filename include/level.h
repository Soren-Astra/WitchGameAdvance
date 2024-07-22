#ifndef __LEVEL_H__
#define __LEVEL_H__

#define collisionTile 0x0001

typedef struct mapInfoTag {
	const unsigned short *mapData;
	const unsigned short *mapPalette;
	const unsigned int *mapTiles;
	const unsigned short *mapCollisions;

	unsigned int mapDataLen;
	unsigned int mapPaletteLen;
	unsigned int mapTilesLen;

	const unsigned int mapWidth;
	const unsigned int mapHeight;
} mapInfo;

void load_level(mapInfo *map, unsigned int playerStartX, unsigned int playerStartY, OBJ_ATTR *obj_buffer);

#endif
