#include "thread.h"
extern PCB* current;

void
new_sem(Semaphore *sem,int value){
        sem->count=value;
        list_init(&sem->queue);
}

void
P(Semaphore *sem){
        lock();
        NOINTR;
        sem->count--;
        if(sem->count < 0){
	        list_add_before(&sem->queue,&current->semq);
                sleep();
	}
	unlock();
}

void 
V(Semaphore *sem){
	lock();
        NOINTR;
	sem->count++;
        if(sem->count <= 0){ 
	        assert(!list_empty(&sem->queue));
	        PCB *pcb=list_entry(sem->queue.next,PCB,semq);
	        list_del(sem->queue.next);
	        wakeup(pcb);
       }
       unlock();
}

