#include "thread.h"
#include "debug.h"
#include "x86.h"

static Message msg_pool[NR_MSG];
static uint32_t ptr;
static ListHead message_avaq;
static ListHead message_freeq;
static boolean wait_flag[NR_MSG];


void
messq_init(void){
	int i = 0;
	list_init(&msgq);
	for (i; i < NR_MES; i++ )
	      list_add_before(&message_freeq,&msg_pool[i].freeq);
	assert( !list_empty(&message_freeq) );
}

void 
copy_message(Message* dst,Message *src) {
	memcpy((void *)dst,(void *)src,sizeof(Message));
}
void 
put_message(Message *src) {
	Message *msg;
	if(list_empty(&message_freeq) )
	      panic("Message queue full!");
	msg=list_entry(&message_freeq->next,Message,freeq);
	list_del(msg->freeq);
	list_add_before(&message_avaq,msg->avaq);
	copy_message(msg,src);
}

boolean
search_message(pid_t pid) {
	Message *msg;
	ListHead *it;
	assert(!list_empty(&message_avaq));

	list_foreach(it,&message_avaq) {
		msg=list_entry(it,Message,freeq);
		if(msg->dst == pid ){
			
		      copy_message(m,msg);
		      list_del(msg->freeq);
		      return msg;
		}
	}
	return NULL;
}
void 
take_message(Message *dst) {
	Message *src=search_message(current->pid);
	assert(!msg);
        copy_message(dst,src);
}








	
/*
find_message(pid_t pid) {
	if
void
add_message(Message* m) {
	if (ptr == NR_MSG)
	      panic("Message queue full!\n");
	copy_message(&pcb_pool[ptr++],m);
}

void
take_message() {
	assert(ptr < NR_MSG);
	copy_message(m,
*/

ListHead*
get_message(pid_t pid){
	ListHead *it;
	list_foreach(it,&msgq) {
		if( pid == it->dest )
		     return it;
	return NULL;
	}	
}

void 
send(pid_t pid, Message *m){
	PCB *pcb=find_pcb_pid(pid);
	if( pcb == NULL )
	      return;
	if( pcb->message.count<0 )
	      copy_message(pcb->message_addr,m,sizeof(Message));
	else
	      put_message(m);
	V(&pcb->message);
}

void 
receive(pid_t pid,Message *m){
	if(current->message.count > 0){
		P(&current->message);
		take_message(m);
	}
	else{
		current->message_addr=m;
		P( &current->message);
	}
}








}
void 
receive(pid_t pid,Message *m){
	Message *msg;
	ListHead *it = get_message(current->pid);
	if(!ListHead){
		mes=list_entry(it,Message,
		

		

