/*
 * Dogadjaj.cpp
 *
 *  Created on: Jun 4, 2016
 *      Author: OS1
 */


#include "Dogadjaj.h"
#include <dos.h>
#include <stdlib.h>
#include "IVTEntry.h"
#include "schedule.h"

//KernelEv* KernelEv::lista = NULL;

KernelEv::KernelEv(IVTNo u){
	ulaz = u;
	myPCB = PCB::running;
}

KernelEv::~KernelEv(){
	IVTEntry::dohvatiIVT(ulaz)->restore();
}

void KernelEv::signal(){
	lock
	if(myPCB != NULL && myPCB->blocked){
		myPCB->blocked = 0;
		Scheduler::put((PCB*)myPCB);
	}
	unlock
}

void KernelEv::wait(){
	lock
	if(myPCB != NULL && myPCB == PCB::running && myPCB->blocked == 0){
		myPCB->blocked = 1;
		dispatch();
	}
	unlock
}
