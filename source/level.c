#include <string.h>
#include <tonc.h>
#include "level.h"
#include "toolbox.h"
#include "logs.h"

void render_screen(const unsigned short *map, 
	unsigned int mapWidth, unsigned int mapHeight, 
	int posX, int posY)
{
	// Center screen on position
	posX = clamp(posX - screenCenterX, -1, mapWidth - screenWidth);
	posY = clamp(posY - screenCenterY, -1, mapHeight - screenHeight);
	
	// Align position to tile grid
	posX = posX - (posX % 16);
	posY = posY - (posY % 16);

	// Position on pixel map to position on tilemap
	posX = posX / 16; //(posX * 100 / 1024) * 64 / 100
	posY = posY / 8; //(posY * 100 / 1024) * 128 / 100
	
	int offset = posY * 128 + posX * 2;
	
	for(int i = 0; i < 23; i++)
	{
		memcpy(&se_mem[8][i * 32], &map[offset], 70);
		offset += 128;
	}
}

int set_spriteId(unsigned int *spriteId)
{
	if(key_hit(KEY_DOWN))
	{
		*spriteId = 0;
	}
	if(key_hit(KEY_UP))
	{
		*spriteId = 16;
	}
	if(key_hit(KEY_LEFT))
	{
		*spriteId = 32;
	}
	if(key_hit(KEY_RIGHT))
	{
		*spriteId = 48;
	}
	return 0;
}

void manage_player(mapInfo *map, unsigned int playerStartX, unsigned int playerStartY, OBJ_ATTR *obj_buffer)
{
	// Position of the player on the global map
	unsigned int playerMapX = playerStartX, playerMapY = playerStartY;
	// Position of the player on the screen
	int playerScreenX = screenCenterX - 16, playerScreenY = screenCenterY - 16;
	// Background offset
	int bgX = 0, bgY = 0;
	u32 spriteId = 0, paletteBank = 0;
	// Map size
	int mapSizeX = map->mapWidth, mapSizeY = map->mapHeight;
	OBJ_ATTR *witch= &obj_buffer[0];
	obj_set_attr(witch,
		ATTR0_SQUARE,
		ATTR1_SIZE_32,
		ATTR2_PALBANK(paletteBank) | spriteId);

	obj_set_pos(witch, playerScreenX, playerScreenY);
 	char nb[20];
	while(1)
	{
		vid_vsync();
		key_poll();

		playerMapX += key_tri_horz() * 2;
		playerMapX = clamp(playerMapX, 0, mapSizeX);
		playerMapY += key_tri_vert() * 2;
		playerMapY = clamp(playerMapY, 0, mapSizeY);
		
		set_spriteId(&spriteId);
		if(key_hit(KEY_A))
		{
			playerMapX++;
		}
		if(key_hit(KEY_B))
		{
			playerMapX--;
		}

		// Set player position on screen
		//Is it supposed to be < or <= ?
		if (playerMapY < screenCenterY || playerMapY > (mapSizeY - screenCenterY))
		{
			//If top, start from 0, if bottom start from middle
			playerScreenY = screenCenterY * (playerMapY / (mapSizeY - screenCenterY));
			//Add actual player offset
			playerScreenY += playerMapY % (mapSizeY - screenCenterY);
			//Recenter sprite
			playerScreenY -= 16;
		}
		else
		{
			playerScreenY = screenCenterY - 16;
		}
		if (playerMapX < screenCenterX || playerMapX > (mapSizeX - screenCenterX))
		{
			//If left border, start from 0, if right border start from middle
			playerScreenX = screenCenterX * (playerMapX / (mapSizeX - screenCenterX));
			//Add actual player offset
			playerScreenX += playerMapX % (mapSizeX - screenCenterX);
			//Recenter sprite
			playerScreenX -= 16;
		}
		else
		{
			playerScreenX = screenCenterX - 16;
		}

		// log position
		sprintf(nb, "%d, %d", playerMapX, playerMapY);
		log_msg(nb);
		
		// Display the map
		render_screen(map->mapData, map->mapWidth, map->mapHeight, playerMapX, playerMapY);
		if (playerMapX >= (mapSizeX - screenCenterX))
		{
			bgX = 16;
		}
		else if (playerMapX < screenCenterX)
		{
			bgX = 0;
		}
		else
		{
			bgX = max((playerMapX - screenCenterX) % 16, 0);
		}
		if(playerMapY >= (mapSizeY - screenCenterY))
		{
			bgY = 16;
		}
		else if (playerMapY < screenCenterY)
		{
			bgY = 0;
		}
		else
		{
			bgY = max((playerMapY - screenCenterY) % 16, 0);
		}
		// Apply the rest
		witch->attr2= ATTR2_BUILD(spriteId, paletteBank, 0);
		obj_set_pos(witch, playerScreenX, playerScreenY);
		oam_copy(oam_mem, obj_buffer, 1);
		REG_BG1HOFS= bgX;
		REG_BG1VOFS= bgY;
	}
}

void load_level(mapInfo *map, unsigned int playerStartX, unsigned int playerStartY, OBJ_ATTR *obj_buffer)
{
	memcpy(&tile_mem[0][0], map->mapTiles, map->mapTilesLen);
	memcpy(pal_bg_mem, map->mapPalette, map->mapPaletteLen);

	manage_player(map, playerStartX, playerStartY, obj_buffer);
}

