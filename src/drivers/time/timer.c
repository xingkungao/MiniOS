#include "common.h"
#include "assert.h"
#include "x86.h"
#include "debug.h"
#include "string.h"
#include "headers/time.h"
#include "thread.h"

extern void add_irq_handle(uint32_t irq,void (*ptr)(void));
extern boolean need_sched;
//pid_t TIME;

long jiffy = 0;
void init_timer(void);
void timerd(void);
static Time rt;

void init_i8253(void);
void update_sched(void);
void update_jiffy(void);
int read_rtc(int);

void init_timer(void) {
	init_i8253();
	add_irq_handle(0, update_sched);
	add_irq_handle(0, update_jiffy);

	int tmp;
	do {
		rt.second = read_rtc(0);
		rt.minute = read_rtc(2);
		rt.hour = read_rtc(4);
		rt.day = read_rtc(7);
		rt.month = read_rtc(8);
		rt.year = read_rtc(9) + 2000;
		tmp = read_rtc(0);
	} while (tmp != rt.second);

};

static int md(int year, int month) {
	boolean leap = (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
	static int tab[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	return tab[month] + (leap && month == 2);
}

void update_jiffy(void) {
	jiffy ++;
	if (jiffy % HZ == 0) {
		rt.second ++;
		if (rt.second >= 60) { rt.second = 0; rt.minute ++; }
		if (rt.minute >= 60) { rt.minute = 0; rt.hour ++; }
		if (rt.hour >= 24)   { rt.hour = 0;   rt.day ++;}
		if (rt.day >= md(rt.year, rt.month)) { rt.day = 1; rt.month ++; } 
		if (rt.month >= 13)  { rt.month = 1;  rt.year ++; }
	}
}

void update_sched(void) {
	need_sched = TRUE;
}

void init_i8253(void) {
	int count = FREQ_8253 / HZ;
	assert(count < 65536);
	out_byte(PORT_TIME + 3, 0x34);
	out_byte(PORT_TIME    , count % 256);
	out_byte(PORT_TIME    , count / 256);	
}

int read_rtc(int reg) {
	out_byte(PORT_RTC, reg);
	int ret = in_byte(PORT_RTC + 1);
	return (ret & 0xf) + (ret >> 4) * 10;
}

void get_time(Time *tm) {
	lock();
	memcpy(tm, &rt, sizeof(Time));
	unlock();
}
