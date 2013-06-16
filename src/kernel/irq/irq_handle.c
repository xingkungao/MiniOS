#include "x86.h"
#include "kernel.h"
#include "thread.h"
#include "adt/list.h"

extern  PCB *current;
boolean intr_flag;
static int32_t nest_level;

static uint32_t isr_count=0;
volatile boolean need_sched=FALSE;

static void (*do_timer_1) (void);
static void (*do_timer_2) (void);
static void (*do_timer_3) (void); 
static void (*do_keyboard) (void); 
void add_irq_handle(uint32_t irq, void (*ptr)(void)){
	assert(irq >= 0);
	if(irq == 0) {
		if(isr_count == 0)
		      do_timer_1=ptr;
		else if(isr_count == 1)
		      do_timer_2=ptr;
		else if(isr_count == 2) 
		      do_timer_3=ptr;
		isr_count++;
	}
	if(irq == 1){
	      do_keyboard=ptr;
	}
}
/*
void irq_handle(TrapFrame *tf) {

	int irq = tf->irq;
	assert(irq >= 0);

	cli();
	intr_flag=TRUE;
	current->tf = tf;

	if(irq <1000 && irq !=0x80) {
		printk("Unexpected exception #%d\n", irq);
		printk(" errorcode %x\n", tf->err);
		printk(" location  %d:%x, esp %x\n", tf->cs, tf->eip, tf);
		panic("unexpected exception");	
	}
	else if(irq == 0x80) {
//		printk("					now comes a int 80   exception\n");
	        schedule();
	}

	else if(irq == 1000) {
//		printk("now comes a time     interrupt!\n");
		do_timer_1();
		do_timer_2();
		do_timer_3();
		do_timer_1();
		if(need_sched){
			need_sched=FALSE;
	      		schedule();
		}
	}
	else if (irq == 1001) {
//		printk("										now comes a keyboard interrupt!\n");
		do_keyboard();
	      	schedule();
	}
	intr_flag=FALSE;
	
}

*/
void irq_handle(TrapFrame *tf) {
	TrapFrame *temp;
	int irq = tf->irq;
	assert(irq >= 0);

	cli();
	nest_level++;
	intr_flag=TRUE;
	current->tf = tf;
	temp=current->tf;

	if(irq <1000 && irq !=0x80) {
		printk("Unexpected exception #%d\n", irq);
		printk(" errorcode %x\n", tf->err);
		printk(" location  %d:%x, esp %x\n", tf->cs, tf->eip, tf);
		panic("unexpected exception");	
	}
	sti();
	//pay attention to the protection of critical section 
	//in ISR since nested interrupt is allowed.
	


	//In nested interrupt, schedule is never called for
	//the protectoin of the excuting order.

	if(irq == 0x80) {
		cli();
		current->tf=temp;
		if(nest_level == 1)
		      schedule();
	}

	else if(irq == 1000) {
		lock();
		do_timer_1();
		do_timer_2();
		do_timer_3();
		unlock();

		cli();
		current->tf=temp;
		if(need_sched){
			need_sched=FALSE;
			if(nest_level == 1)
			      schedule();
		}
	}

	else if (irq == 1001) {
		do_keyboard();
		cli();
		current->tf=temp;
		if(nest_level == 1)
		      schedule();
	}

	if(nest_level == 1)
		intr_flag=FALSE;

	nest_level--;

	
}
