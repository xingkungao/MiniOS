#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "common.h"
#include "assert.h"

void init_debug(void);
int printk(const char *fmt, ...);

#endif
