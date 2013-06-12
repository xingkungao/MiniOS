#include "thread.h"
#include "string.h"
#include "message.h"
#include "debug.h"
#include "x86.h"
#include "kernel.h"

static Message msg_pool[NR_MSG];
Message *front=&msg_pool[0];
Message *rear=&msg_pool[0];

static size_t
length_of_msgq() {
	return ((unsigned)rear-(unsigned)front)/sizeof(Message);
}

static void 
copy_message(Message* dst,Message *src) {
       	memcpy((void *)dst,(void *)src,sizeof(Message));
}

static void 
put_message(Message *src) {
	if(length_of_msgq() == NR_MSG)
	     panic("no room for more message!\n"); 
	copy_message(rear,src);
	rear++;
}

static Message*
search_message(pid_t src_id,pid_t dst_id) {
	Message *msg;
	assert(length_of_msgq());
	if(src_id == ANY){
		for( msg = front; msg < rear; msg++ ) {
			if(msg->dest == dst_id)
			      return msg;
		}
	}
	else{
		for (msg = front;msg < rear;msg++) {
			if(msg->dest == dst_id && msg->src == src_id ){
		      		return msg;
			}
		}
	}
	return NULL;
}
static void 
take_message(pid_t pid,Message *dst) {
	assert(0 != length_of_msgq());
	Message *src=search_message(pid,current->pid);
	Message *ptr=src;
	assert(src!=NULL);
	
        copy_message(dst,src);
	for (;ptr < rear;ptr++)
	      *(ptr)=*(ptr+1);
	rear--;
}

void 
send(pid_t pid, Message *m){
	m->src = current->pid;
	m->dest = pid;
	if( m->src == MSG_HWINTR)
	      printk("source is msg_hwintr\n");
	lock();
	PCB *pcb=find_pcb_pid(pid);
	if( pcb == NULL ){

		printk("send found no pcb by pid");
		unlock();  
	      	return;
	}
	if( pcb->message.count <= -1 ){
		printk("send while waiting,derectly copy %d->%d\n",current->pid,pid);
	      copy_message(pcb->message_addr,m);
	}
	else{
		printk("send while not waiting,enqueue %d->%d\n",current->pid,pid);
		printk("will put msg\n");
	      put_message(m);
	}
	V(&pcb->message);
	NOINTR;
	unlock();
}

void 
receive(pid_t pid,Message *m){
	lock();
	NOINTR;
	if(current->message.count >= 1){
		P(&current->message);
		printk("send: have sems: %d\n",current->message.count);
		printk("try receive while dequeue %d<-%d\n",current->pid,pid);
		NOINTR;
		take_message(pid,m);
		printk("have receive from dequeue %d<-%d\n",m->dest,m->src);
	}
	else{
		current->message_addr=m;
		printk("receive directly %d<-%d\n",current->pid,pid);
		P( &current->message);
	}
	unlock();
}
