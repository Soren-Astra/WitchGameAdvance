#include <string.h>
#include <tonc.h>
#include "witch.h"
#include "dragonruinstest.h"
#include "logs.h"

OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer= (OBJ_AFFINE*)obj_buffer;

void render_screen(int posX, int posY)
{
	// center
	posX = max(posX - 120, 0);
	posY = max(posY - 80, 0);
	posX = posX - (posX % 16);
	posY = posY - (posY % 16);
	//posX = (posX * 100 / 1024) * 64 / 100;
	//posY = (posY * 100 / 1024) * 128 / 100;
	posX = posX / 16;
	posY = posY / 8;
	int offset = posY * 128 + posX * 2;
	for(int i = 0; i < 23; i++)
	{
		memcpy(&se_mem[8][i * 32], &dragonruinstestMap[offset], 70);
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

void manage_player()
{
	// Position of the player on the global map
	int playerMapX = 516, playerMapY = 660;
	// Position of the player on the screen
	int playerScreenX = 104, playerScreenY = 64;
	// Background offset
	int bgX = 0, bgY = 0, sbOffset = 0;
	u32 spriteId = 0, paletteBank = 0;
	// Map size
	int mapSizeX = 1024, mapSizeY = 1024;
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
			sbOffset += 1024;
		}

		// Set player position on screen
		if (playerMapY < 64 || playerMapY > (mapSizeY - 64))
		{
				// Math for the sides
				playerScreenY = (64 * (playerMapY / (mapSizeY - 64))) + (playerMapY % (mapSizeY -64));
		}
		else
		{
				playerScreenY = 64;
		}

		// log position
		sprintf(nb, "%d, %d", playerMapX, playerMapY);
		log_msg(nb);
		
		// Display the map
		render_screen(playerMapX, playerMapY);
		bgX = (playerMapX - 120) % 16;
		bgY = (playerMapY - 80) % 16;

		// Apply the rest
		witch->attr2= ATTR2_BUILD(spriteId, paletteBank, 0);
		obj_set_pos(witch, playerScreenX, playerScreenY);
		oam_copy(oam_mem, obj_buffer, 1);
		REG_BG1HOFS= bgX;
		REG_BG1VOFS= bgY;
	}
}

void init_graphics()
{
	//player
	memcpy(&tile_mem[4][0], witchTiles, witchTilesLen);
	memcpy(pal_obj_mem, witchPal, witchPalLen);


	//bg
	memcpy(&tile_mem[0][0], dragonruinstestTiles, dragonruinstestTilesLen);
	memcpy(pal_bg_mem, dragonruinstestPal, dragonruinstestPalLen);
	
	REG_BG1CNT= BG_CBB(0) | BG_SBB(8) | BG_4BPP | BG_REG_64x32;
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;
}

int main()
{
	init_graphics();
	init_logs(2, 30);
	oam_init(obj_buffer, 128);

	manage_player();

	while(1);

	return 0;
}
