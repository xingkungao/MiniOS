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
void A(void);
void B(void);

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
	sti();
	while (TRUE) {
		wait_intr();
	}
}
void
A(void){

		int a=0;
	while(1){
		Message m1,m2;
		m1.type=a++;
		send(2,&m1);
		m2.type=a++;
		printk("					A:a send to b m1\n");
		send(2,&m2);
		printk("					A:a send to b m2\n");
		receive(2,&m1);
		printk("							A:a rece fr b m1  %d\n",m1.type);
		receive(2,&m2);
		printk("							A:a rece fr b m2  %d\n",m2.type);
	}
}
void
B(void){
	while(1){
		Message m;
		receive(1,&m);
		printk("B:					b try receive from a %d\n",m.type);
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
