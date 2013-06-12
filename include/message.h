#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "adt/list.h"
#define NR_MSG 128
struct Message{
	pid_t src,dest;
	size_t type;
	ListHead avaq;
	ListHead freeq;
	char payload[8];
};
typedef struct Message Message;

void messq_init(void);
void send(int pid, struct Message *m);
void receive(int pid, struct Message *m);

#endif
