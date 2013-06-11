#include "x86.h"
#include "kernel.h"
#include "thread.h"
#include "adt/list.h"

extern  PCB *current;
extern  PCB pcb[NR_PCB];
extern  ListHead *runq_head;

static uint32_t nestlevel;
static uint32_t isr_count;
static boolean need_sched=FALSE;

static void (*do_timer_1) (void);
static void (*do_timer_2) (void);
static void (*do_timer_3) (void);
static void (*do_keyboard) (void);

void add_irq_handle(int irq, void (*ptr) (void)){
	if(irq == 0) {
		if(isr_count++ == 0)
		      do_timer_1=ptr;
		else if(isr_count++ == 1)
		      do_timer_2=ptr;
		else 
		      do_timer_3=ptr;
	}
	if(irq == 1)
	      do_keyboard=ptr;
}

void irq_handle(TrapFrame *tf) {
	cli();
	nestlevel++;
	sti();

	TrapFrame *temp;
	int irq = tf->irq;
	assert(irq >= 0);

	if(irq <1000 && irq !=0x80) {
		cli();
		printk("Unexpected exception #%d\n", irq);
		printk(" errorcode %x\n", tf->err);
		printk(" location  %d:%x, esp %x\n", tf->cs, tf->eip, tf);
		panic("unexpected exception");	
	}
	else if(irq == 0x80) {
		cli();
		if(nestlevel == 1)
		      schedule();
		sti();
	}
	else if(irq == 1000) {
		do_timer_1();
		do_timer_2();
		do_timer_3();
	}
	else if (irq == 1001) {
		do_keyboard();
	}

	cli();
	nestlevel--;
	sti();
}


/*
	if (irq >= 1000 || irq == 0x80 ) {

		// int 0x80 exception or external interrupt
		cli();

	
//		if(irq == 0x80)
//		      printk("now triggers a 0x80 exception!\n");
//		else
//		      printk("now comes a interrupt!\n");
	

		//store the exact location in the pcb of the current thread's trapframe
		current->tf=tf;
		temp=tf;
		sti();

		cli();
		current->tf=temp;
	//	sti();

		//switch current to next pcb in runq_head. Use "FCFS" strategy for scheduling. 
		
		//WARNNING: the following code is actually a little bit unsafe because the current->runq
		//may have been deleted from runq_head after the routine "sleep()" is called.
		//But due to the implementation of the routine "list_del", current->runq.next
		//still points to the next pcb->runq we need properly. 

	//	cli();
		current=list_entry((current->runq).next,PCB,runq);

		//notice that theread idle(i.e. pcb[0]) always stays in the list "runq_head";
		//to avoid choosing idle while other thread is available to schedule,
		//we must "skip" the idle thread. 
		if(current==&pcb[0])
	              current=list_entry((current->runq).next,PCB,runq);
	}
	else{
	 	//exceptions 	
		if( irq == 0x80 ){
			cli();
			current->tf=tf;
			current=list_entry((current->runq).next,PCB,runq);
			if(current==&pcb[0])
	              		current=list_entry((current->runq).next,PCB,runq);
		}
		else{
			cli();
			printk("Unexpected exception #%d\n", irq);
			printk(" errorcode %x\n", tf->err);
			printk(" location  %d:%x, esp %x\n", tf->cs, tf->eip, tf);
			panic("unexpected exception");
		}
	}
}
*/
