#include "debug.h"
#include "x86.h"

#define SERIAL_PORT  0x3F8

void
init_debug(void) {
	out_byte(SERIAL_PORT + 1, 0x00);
	out_byte(SERIAL_PORT + 3, 0x80);
	out_byte(SERIAL_PORT + 0, 0x01);
	out_byte(SERIAL_PORT + 1, 0x00);
	out_byte(SERIAL_PORT + 3, 0x03);
	out_byte(SERIAL_PORT + 2, 0xC7);
	out_byte(SERIAL_PORT + 4, 0x0B);
}

static inline boolean
serial_idle(void) {
	return (in_byte(SERIAL_PORT + 5) & 0x20) != 0;
}

void
serial_printc(char ch) {
	while (serial_idle() != TRUE);
	out_byte(SERIAL_PORT, ch);
}

int vfprintf(void (*)(char), const char *, void **);

int
printk(const char *fmt, ...) {
	void **args = (void **)&fmt + 1;
	return vfprintf(serial_printc, fmt, args);
}

