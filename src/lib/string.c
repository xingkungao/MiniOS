#include "common.h"
#include "string.h"
#include "assert.h"

void *
memcpy(void *dst, const void *src, size_t count) {
	void *ret = dst;
	while (count --) {
		*(char *)dst = *(char *)src;
		dst = (char *)dst + 1;
		src = (char *)src + 1;
	}
	return ret;
}

void *
memset(void *dst, int val, size_t count) {
	void *start = dst;
	while (count --) {
		*(char *)dst = (char)val;
		dst = (char *)dst + 1;
	}
	return start;
}

char *
itoa_buf(int value, char *buf, int base) {
	boolean negative = FALSE;
	if (value < 0) {
		negative = TRUE;
		value = -value;
	}
	buf = utoa_buf(value, buf + 1, base);
	if (negative) {
		*(--buf) = '-';
	}
	return buf;
}

char *
utoa_buf(unsigned int value, char *buf, int base) {
	static char digits[] = "0123456789abcdef";
	assert(base > 1 && base <= 16);
	*(buf += 34) = 0;
	do {
		*(--buf) = digits[value % base];
		value /= base;
	} while (value > 0);
	return buf;
}

int
toupper(int ch) {
	return (ch >= 'a' && ch <= 'z') ? (ch - 'a' + 'A') : (ch);
}
int
tolower(int ch) {
	return (ch >= 'A' && ch <= 'Z') ? (ch - 'A' + 'a') : (ch);
}

char *
itoa(int value, int base) {
	static char buf[50];
	return itoa_buf(value, buf, base);
}

char *
utoa(unsigned int value, int base) {
	static char buf[50];
	return utoa_buf(value, buf, base);
}
