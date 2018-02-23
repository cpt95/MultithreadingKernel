/*
 * Covece.cpp
 *
 *  Created on: Jun 3, 2016
 *      Author: OS1
 */




#include "Covece.h"
#include "PCB.h"
#include "Jezgro.h"
#include <iostream.h>
#include "Schedule.h"

volatile Lista* KernelSem::sleepQueue = new Lista();

KernelSem::KernelSem(int init){
	value = init;
	waitQueue = new Lista();
}

KernelSem::~KernelSem(){
	delete waitQueue;
}

int KernelSem::wait(int time){
	/*
	lock
	value--;
	if(value<0){

		PCB::running->blocked = 1;
		if(time > 0){
			value++;
			PCB::running->waitTime = time;
			//KernelSem::sleepQueue->dodaj(PCB::running);
			KernelSem::sleepQueue->dodajUSleep(PCB::running);
		}else{
			waitQueue->dodaj(PCB::running);
		}
		dispatch();
	}
	unlock
	if(PCB::running->waitTime  == -2) return 1;
	return 0;
	*/


	lock
	if(--value < 0){
		PCB::running->blocked = 1;
		PCB::running->sem = this;
		waitQueue->dodaj(PCB::running);
		if(time == 0){
			dispatch();
			unlock
			return 1;
		}else{
			PCB::running->waitTime = time;
			KernelSem::sleepQueue->dodajUSleep(PCB::running);
			dispatch();
			if(PCB::running->waitTime > 0){
				unlock
				return 1;
			}else{
				unlock
				return 0;
			}
		}
	}
	unlock
	return 0;

}

void KernelSem::signal(){
	lock
	if(value++<0){
		PCB* tmp = waitQueue->izbaciPrvi();

		//if(tmp->waitTime > 0){
		//	KernelSem::sleepQueue->izbaci(tmp);
		//}
		tmp->blocked = 0;
		//tmp->waitTime= -2;
		Scheduler::put((PCB*)tmp);



	}
	unlock
}
int KernelSem::val() const{
	return value;
}

