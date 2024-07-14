#include <tonc.h>

void init_logs(int cbb, int sbb)
{
	REG_DISPCNT= REG_DISPCNT | DCNT_BG0;
	tte_init_se_default(0, BG_CBB(cbb)|BG_SBB(sbb));
}

void log_msg(char *msg)
{
	tte_write("#{P:0,128}");
	tte_write("#{el}");
	tte_write(msg);
}
