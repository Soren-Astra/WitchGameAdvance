#include <tonc.h>
#include <string.h>
#include "witch.h"
#include "level.h"
#include "logs.h"
#include "dragonruinstest.h"
#include "dragonruinsdata.h"

OBJ_ATTR obj_buffer[128];

void init_graphics()
{
	//player
	memcpy(&tile_mem[4][0], witchTiles, witchTilesLen);
	memcpy(pal_obj_mem, witchPal, witchPalLen);

	//bg
	REG_BG1CNT= BG_CBB(0) | BG_SBB(8) | BG_4BPP | BG_REG_64x32;
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;
}

int main()
{
	init_graphics();
	init_logs(2, 30);
	oam_init(obj_buffer, 128);
	mapInfo level = { dragonruinstestMap, dragonruinstestPal, dragonruinstestTiles,
		dragonruinstestMapLen, dragonruinstestPalLen, dragonruinstestTilesLen, 
		dragonruinsPixelWidth, dragonruinsPixelHeight };
	load_level(&level, 450, 450, obj_buffer);

	while(1);

	return 0;
}
