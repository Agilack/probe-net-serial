#include "hardware.h"
#include "time.h"

static u32 time_count;
static u32 utime_count;
static u8  offset;

void time_init(void)
{
	time_count = 0;
	utime_count = 0;
	offset = 0;

	/* Configure Systick */
	reg_wr(SYSTICK_CTRL,  0x04); /* Set core clock as source */
	reg_wr(SYSTICK_LOAD, 80000); /* Approx 10ms              */
	reg_set(SYSTICK_CTRL, 0x01); /* Enable systick           */
	reg_set(SYSTICK_CTRL, 0x02); /* Activate tick interrupt  */
}

u32 get_time(void)
{
	return time_count;
}

u32 get_utime(void)
{
	return utime_count;
}

void SysTick_Handler(void)
{
	utime_count++;

	offset++;
	if (offset == 100)
	{
		time_count ++;
		offset = 0;
	}
}
