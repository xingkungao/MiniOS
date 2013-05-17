#include "kernel.h"
#include "x86.h"
#include "vm.h"
#include "irq.h"
#include "thread.h"

void thread_tb1(void);
void idle(void);
void thread_tb0(void);
void thread_tb2(void);
void thread_tb3(void);
void thread_tb4(void);
void test_producer(void);
void test_consumer(void);
void test_setup(void);

extern ListHead *runq_head;
extern PCB*	current;

#define NBUF 5
int buf[NBUF], f = 0, r = 0, g = 1, tid = 1;
int pid=1;
Semaphore empty, full, mutex;

void
os_init(void) {
	init_seg();
	init_debug();
	init_idt();
	init_i8259();
	init_pcbpool();
	test_setup();
/*
	wakeup(create_kthread((void*)thread_tb0));
	wakeup(create_kthread((void*)thread_tb1));
	wakeup(create_kthread((void*)thread_tb2));
	wakeup(create_kthread((void*)thread_tb3));
	wakeup(create_kthread((void*)thread_tb4));
*/
	idle();
}

void
entry(void) {
	init_kvm();
	void(*volatile next)(void) = os_init;
	asm volatile("addl %0, %%esp" : : ""(KOFFSET));
	next();
	panic("init code should never return");
}

void
thread_tb0(void){
	while (TRUE) {
		printk("now excute thread_0!\n");
	}
}

void
thread_tb1(void){
	while(TRUE)
	printk("now excute thread_1!\n");
}
void
thread_tb2(void){
	while(TRUE)
	{
	printk("now excute thread_2!\n");
	sleep();
	}
}
void
thread_tb3(void){
	while (TRUE) {
	printk("now excute thread_3!\n");
	}
}
void
thread_tb4(void){
	while (TRUE) {
	printk("now excute thread_4!\n");
	}
}

void
idle(void){
	printk("\n\nnow excute idle!\n");
	sti();
	while (TRUE) {
		wait_intr();
	}
}


void
test_producer(void) {
	int id =pid ++;
	while (TRUE) {
        	P(&empty);
		INTR;
		P(&mutex);
		INTR;
		printk("#%d Pro: %d\n", id, g);
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
