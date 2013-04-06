#ifndef __STRING_H__
#define __STRING_H__

#include "types.h"

void *memcpy(void *dst, const void *src, size_t count);
void *memset(void *dst, int val, size_t count);

char *itoa(int value, int base);
char *itoa_buf(int value, char *buf, int base);
char *utoa(unsigned int value, int base);
char *utoa_buf(unsigned int value, char *buf, int base);

int toupper(int ch);
int tolower(int ch);

#endif

