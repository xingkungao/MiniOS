#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "adt/list.h"
#define NR_MSG 512 
struct Message{
	pid_t src,dest;
	size_t type;
//	ListHead avaq;
//	ListHead freeq;
	char payload[16];
};
typedef struct Message Message;

void send(int pid, struct Message *m);
void receive(int pid, struct Message *m);

#endif
