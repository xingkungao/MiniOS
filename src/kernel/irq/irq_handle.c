#include "x86.h"
#include "kernel.h"
#include "thread.h"
#include "adt/list.h"

extern  PCB *current;
extern  PCB pcb[NR_PCB];
extern  ListHead *runq_head;

//static uint32_t nestlevel;
uint32_t isr_count=0;
boolean need_sched=FALSE;

static void (*do_timer_1) (void);
static void (*do_timer_2) (void);
static void (*do_timer_3) (void);
static void (*do_keyboard) (void);

void add_irq_handle(uint32_t irq, void (*ptr)(void)){
	assert(irq >= 0);
	if(irq == 0) {
	printk("%d\n",isr_count);
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
	      printk("add key \n");
	}
}

void irq_handle(TrapFrame *tf) {
	//volatile pid_t temp;
	pid_t temp;
	cli();
//	nestlevel++;

//	TrapFrame *temp;
	int irq = tf->irq;
	assert(irq >= 0);
	current->tf = tf;

	if(irq <1000 && irq !=0x80) {
		printk("Unexpected exception #%d\n", irq);
		printk(" errorcode %x\n", tf->err);
		printk(" location  %d:%x, esp %x\n", tf->cs, tf->eip, tf);
		panic("unexpected exception");	
	}
	else if(irq == 0x80) {
		printk("now comes a int 80   exception\n");
	      	schedule();
	}

	else if(irq == 1000) {
		printk("now comes a time     interrupt!\n");
		temp = current->pid ;
		current->pid = MSG_HWINTR;
		do_timer_1();
		do_timer_2();
		do_timer_3();
		current->pid =temp;
	if(need_sched){
		printk("time to schedule\n");
		need_sched=FALSE;
	      schedule();
	}
	}
	else if (irq == 1001) {
		printk("now comes a keyboard interrupt!\n");
		temp = current->pid ;
		current->pid = MSG_HWINTR;
		do_keyboard();
		current->pid =temp;
	}
/*	if(need_sched){
		printk("time to schedule\n");
		need_sched=FALSE;
	      schedule();
	}
	*/
	//cli();
	//nestlevel--;
	//sti();
}


/*
void irq_handle(TrapFrame *tf) {
	int irq = tf -> irq;
	assert(irq >= 0);
	if (irq >= 1000 || irq == 0x80 ) {
	printk("now comes a interrupt!\n");

		// int 0x80 exception or external interrupt
		cli();

	
//		if(irq == 0x80)
//		      printk("now triggers a 0x80 exception!\n");
//		else
//		      printk("now comes a interrupt!\n");
	

		//store the exact location in the pcb of the current thread's trapframe
		current->tf=tf;
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
