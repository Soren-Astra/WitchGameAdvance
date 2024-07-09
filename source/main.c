#include <string.h>
#include <tonc.h>
#include "witch.h"
#include "dragonruinstest.h"

OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer= (OBJ_AFFINE*)obj_buffer;
void init_logs()
{
	REG_DISPCNT= REG_DISPCNT | DCNT_BG0;
	tte_init_se_default(0, BG_CBB(2)|BG_SBB(30));
}

void log_msg(char *msg)
{
	tte_write("#{P:0,128}");
	tte_write("#{el}");
	tte_write(msg);
}

int get_screenblock(int x, int y)
{
	return (x / 256) + ((y / 256) * 4);
}

void manage_player()
{
	int x= 510, y= 1010, offset = 0;
	u32 tid= 0, pb= 0;
	OBJ_ATTR *witch= &obj_buffer[0];
	obj_set_attr(witch,
		ATTR0_SQUARE,
		ATTR1_SIZE_32,
		ATTR2_PALBANK(pb) | tid);

	//hard set for now
	obj_set_pos(witch, 104, 64);

	while(1)
	{
		vid_vsync();
		key_poll();

		x += key_tri_horz() * 2;
		y += key_tri_vert() * 2;

		if(key_hit(KEY_DOWN))
		{
			tid = 0;
			if (offset + 4096 < dragonruinstestMapLen / 2)
				offset += 4096;
		}
		if(key_hit(KEY_UP))
		{
			tid = 16;
			if (offset >= 4096)
				offset -= 4096;
		}
		if(key_hit(KEY_LEFT))
		{
			tid = 32;
			if (offset % 4096 != 0)
				offset -= 1024;
		}
		if(key_hit(KEY_RIGHT))
		{
			tid = 48;
			if ((offset + 1024) % 4096 != 0)
				offset += 1024;
		}
		if(key_hit(KEY_A))
			offset += 2;
		if(key_hit(KEY_B))
			offset += 1024;
		memcpy(&se_mem[8][0], &dragonruinstestMap[offset], 2048);
		witch->attr2= ATTR2_BUILD(tid, pb, 0);
		//obj_set_pos(witch, x, y);

		oam_copy(oam_mem, obj_buffer, 1);	// only need to update one
		//REG_BG0HOFS= x;
		//REG_BG0VOFS= y;
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
	memcpy(&se_mem[8][0], dragonruinstestMap, 2048);
	
	REG_BG1CNT= BG_CBB(0) | BG_SBB(8) | BG_4BPP | BG_REG_32x32;
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG1 | DCNT_OBJ | DCNT_OBJ_1D;
}

int main()
{
	init_graphics();
	init_logs();
	oam_init(obj_buffer, 128);

	manage_player();

	while(1);

	return 0;
}
