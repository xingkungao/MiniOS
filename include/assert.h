#ifndef __ASSERT_H__
#define __ASSERT_H__

void abort(const char *, int);

#define assert(cond) \
	((cond) ? 0 : abort(__FILE__, __LINE__))

#endif
