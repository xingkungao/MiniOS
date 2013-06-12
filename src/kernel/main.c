#include "kernel.h"
#include "x86.h"
#include "vm.h"
#include "irq.h"
#include "thread.h"
#include "semaphore.h"
#include "headers/hal.h"
#include "headers/tty.h"
#include "headers/time.h"

void idle(void);
void test_producer(void);
void test_consumer(void);
void test_setup(void);
//void A(void);
//void B(void);

extern void init_hal(void);
extern void init_timer(void);
extern void init_tty(void);
extern void test(void);
extern void ttyd(void);
extern pid_t TTY;
extern ListHead *runq_head;
//extern PCB*	current;
#define NBUF 5
int buf[NBUF], f = 0, r = 0, g = 1, tid = 1;
//int pid=1;
Semaphore empty, full, mutex;

void
os_init(void) {
	init_seg();
	init_debug();
	init_idt();
	init_i8259();
	init_pcbpool();

	init_hal();
	init_timer();
	init_tty();
	wakeup(set_pid(create_kthread(ttyd),TTY));
	test();


//	wakeup(create_kthread(A));
//	wakeup(create_kthread(B));
	idle();
}

void
entry(void) {
	init_kvm();
	void(*volatile next)(void) = os_init;

	asm volatile("addl %0, %%esp" : : ""(KOFFSET));
	next();
	panic("init code should never return"); }

void
idle(void){
	printk("\n\nnow excute idle!\n");
	sti();
	while (TRUE) {
		wait_intr();
	}
}
void
A(void){
	while(1){
		Message m;
		send(2,&m);
		printk("A:a send to b\n");
		receive(2,&m);
	}
}
void
B(void){
	while(1){
		Message m;
		printk("B:b try receive from a\n");
		receive(1,&m);
		send(1,&m);
	}
}


void
test_producer(void) {
//	int id =pid ++;
	while (TRUE) {
        	P(&empty);
		INTR;
		P(&mutex);
		INTR;
//		printk("#%d Pro: %d\n", id, g);
		buf[f ++] = g ++;
		f %= NBUF;
		V(&mutex);
		INTR;
		V(&full);
		INTR;
	}
}

void
test_consumer(void) {
	int id = tid ++;
	while (TRUE) {
	        P(&full);
		INTR;
	        P(&mutex);
		INTR;
	       	printk("#%d Got: %d\n", id, buf[r ++]);
	        r %= NBUF;
	        V(&mutex);
		INTR;
	        V(&empty);
		INTR;
	}
}

void
test_setup(void) {
        new_sem(&full, 0);
        new_sem(&empty, NBUF);
	new_sem(&mutex, 1);
	wakeup(create_kthread(test_producer));
	wakeup(create_kthread(test_producer));
	wakeup(create_kthread(test_producer));
	wakeup(create_kthread(test_consumer));
	wakeup(create_kthread(test_consumer));
	wakeup(create_kthread(test_consumer));
	wakeup(create_kthread(test_consumer));
}
