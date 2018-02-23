/*
 * Thread.cpp
 *
 *  Created on: May 23, 2016
 *      Author: OS1
 */

#include "Thread.h"
#include "SCHEDULE.h"
#include "PCB.h"
#include "Jezgro.h"
#include <stdlib.h>

void Thread::start(){
	Scheduler::put((PCB*) myPCB);
}

Thread::Thread (StackSize stackSize, Time timeSlice){
	myPCB = new PCB((int)stackSize, (int)timeSlice, this);

}


Thread::~Thread(){
	delete myPCB;
}


void Thread::waitToComplete(){
	if(this->myPCB->zavrsio == 0 && PCB::running!=NULL && myPCB != Jezgro::idle){
		lock
		PCB::running->blocked = 1;
		myPCB->listaCekanja->dodaj(PCB::running);
		dispatch();
		unlock
	}
}

void dispatch(){ // sinhrona promena konteksta
	//asm cli;
	lock
	Jezgro::Zahtevana_promena_konteksta = 1;
	timerInterrupt();
	unlock
	//asm sti;
}


ID Thread::getId(){
	return myPCB->dohvatiID();
}
