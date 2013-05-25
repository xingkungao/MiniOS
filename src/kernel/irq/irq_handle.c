#include "x86.h"
#include "kernel.h"
#include "thread.h"
#include "adt/list.h"


extern  PCB *current;
extern  PCB pcb[NR_PCB];
extern  ListHead *runq_head;

void irq_handle(TrapFrame *tf) {
	int irq = tf->irq;
	assert(irq >= 0);

	if (irq >= 1000 || irq == 0x80) {

		// int 0x80 exception or external interrupt
		cli();

		/*
		if(irq == 0x80)
		      printk("now triggers a 0x80 exception!\n");
		else
		      printk("now comes a interrupt!\n");
	
		*/

		//store the exact location in the pcb of the current thread's trapframe
		current->tf=tf;


		//switch current to next pcb in runq_head. Use "FIFS" strategy for scheduling. 
		
		//WARNNING: the following code is actually a little bit unsafe because the current->runq
		//may have been deleted from runq_head after the routine "sleep()" is called.
		//But due to the implementation of the routine "list_del", current->runq.next
		//still points to the next pcb->runq we need properly. 

		current=list_entry((current->runq).next,PCB,runq);

		//notice that theread idle(i.e. pcb[0]) always stays in the list "runq_head";
		//to avoid choosing idle while other thread is available to schedule,
		//we must "skip" the idle thread. 
		if(current==&pcb[0])
	              current=list_entry((current->runq).next,PCB,runq);
	}
	else{
	 	//exceptions 	

		cli();
		printk("Unexpected exception #%d\n", irq);
		printk(" errorcode %x\n", tf->err);
		printk(" location  %d:%x, esp %x\n", tf->cs, tf->eip, tf);
		panic("unexpected exception");
	}
}
