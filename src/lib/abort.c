#include "debug.h"
#include "x86.h"

void
abort(const char *file, int line) {
	cli();
	printk("\n\nFatal error (assertion failure) at %s:%d\n", file, line);
	wait_intr();
}

void
panic(const char *str) {
	cli();
	printk("\n\nFatal error (kernel panic): %s\n", str);
	wait_intr();
}
