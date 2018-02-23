/*
 * PCB.cpp
 *
 *  Created on: May 23, 2016
 *      Author: OS1
 */

#include "PCB.h"
#include "Thread.h"
#include "SCHEDULE.h"
#include <dos.h>
#include <stdlib.h>
#include "Lista.h"
#include "Jezgro.h"



Lista* PCB::baraba = new Lista();
PCB* PCB::running = 0;
int PCB::idTrenutni = 0;


PCB::PCB(int size, int time, Thread* t){
	size /= sizeof(unsigned int);
	stack = new unsigned int[size];
	stack[size - 1] =0x200;//setovan I fleg u
						  // pocetnom PSW-u za nit
	stack[size - 2] = FP_SEG(PCB::wrapper);
	stack[size - 3] = FP_OFF(PCB::wrapper);

	sp = FP_OFF(stack+size-12); //svi sacuvani registri
	 //pri ulasku u interrupt
	 //rutinu
	ss = FP_SEG(stack+size-12);
	bp = FP_OFF(stack+size-12);


	myThread = t;
	if(time == 0){
		kvant = -1;
	}else{
		kvant = time;
	}
	if(Jezgro::idle == NULL){
		Jezgro::idle = this;
		id = -1;
		zavrsio = 1;
		blocked = 0;
		listaCekanja = NULL;
	}else{
		id = idTrenutni++;
		zavrsio = 0;
		blocked = 0;
		listaCekanja = new Lista();
		baraba->dodaj(this);
	}

}

PCB::PCB(int time){
	id = idTrenutni++;
	zavrsio = 0;
	kvant = time;
	blocked = 0;
	PCB::running = this;
	stack = NULL;
	listaCekanja = new Lista();
	Jezgro::glavni = this;
}

void PCB::wrapper(){
	PCB::running->myThread->run();
	lock
	PCB* tek = PCB::running->listaCekanja->izbaciPrvi();
	while(tek != NULL){
		tek->blocked = 0;
		Scheduler::put(tek);
		tek = PCB::running->listaCekanja->izbaciPrvi();
	}
	PCB::running->zavrsio = 1;
	baraba->izbaci(PCB::running);
	dispatch();
	unlock
}


int PCB::isFinished() const{
	return zavrsio;
}

PCB::~PCB(){
	if(stack != NULL){
		delete stack;
	}
}

int PCB::dohvatiID() const{
	return id;
}





