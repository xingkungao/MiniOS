#include "thread.h"
#include "debug.h"
#include "x86.h"
static Message mess_pool[200];
static ListHead messageq;

void
messq_init(void){
	int i = 0;
	list_init(&messageq);
	for (i = 0; i < NR_MES; i++ )
	      list_add_before(&messageq,&mess_pool[i].messq);
	assert( !list_empty(messageq) );
}

void
get_pcb(pid_t pid){
	ListHead *it;
	list_foreach(it,&messageq) {
		if( pid == it->dest )
		     return TRUE;
	return FALSE;
	}	
}


void 
send(int pid, struct message *m){
	if( list_entry(k

