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

	if (irq < 1000) {
		// exception
		if(irq==0x80){
			cli();
//			printk("now have triggered a 0x80 exception\n");
			current->tf=tf;
		//	current=list_entry(runq_head->prev,PCB,runq);
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
	} else if (irq >= 1000) {

		cli();
		printk("now comes a interrupt!\n");
//		printk("address of previous pcb is        %x\n",(unsigned int)((char*)current));
		current->tf=tf;
//		printk("address of previous pcb->tf is    %x\n",(unsigned int)((char*)current->tf));
/*
		if(list_empty(runq_head)!=1){
			current=list_entry(runq_head->next,PCB,runq);
//			runq_head=runq_head->next;
			list_del(runq_head->next);
		}
		else{
			current=&pcb[0];
		}
		current
*/
		current=list_entry((current->runq).next,PCB,runq);
		if(current==&pcb[0])
		      current=list_entry((current->runq).next,PCB,runq);

//		list_del(&(current->runq));
//		current=list_entry(runq_head->next,PCB,runq);
//		runq_head=runq
//		printk("address of new pcb     is         %x\n",(unsigned int)((char*)current));
//		printk("address of new pcb->tf is         %x\n\n",(unsigned int)((char*)current->tf));

	}
}

