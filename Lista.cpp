/*
 * Lista.cpp
 *
 *  Created on: May 30, 2016
 *      Author: OS1
 */


#include "Lista.h"
#include <stdlib.h>
#include<iostream.h>
#include "schedule.h"
#include "Covece.h"

Elem::Elem(PCB* pp){
	p = pp;
	sledeci = NULL;
}

Lista::Lista(){
	lock
	prvi = NULL;
	unlock
}

void Lista::pisi() volatile{
	lock

	volatile Elem* tmp = prvi;
	while(tmp != NULL){
		ispisi(cout << "ThreadID: " << tmp->p->id << " Blokiran?: "<< tmp->p->blocked <<" WaiTIme?: " << tmp->p->waitTime <<endl)
		tmp = (volatile Elem*)tmp->sledeci;
	}

	unlock
}

void Lista::dodaj(PCB* pp) volatile{
	lock

	if(prvi == NULL){
		//ispisi(cout << "Dodaje se prvi!" << endl)
		prvi = poslednji = new Elem(pp);

	}else{
		//ispisi(cout << "Dodaju se drugi!" << endl)
		poslednji = poslednji->sledeci = new Elem(pp);
	}

	unlock
}

void Lista::izbaci(PCB* pp) volatile{
	lock

	if(prvi != NULL){
		volatile Elem* tek = prvi;
		volatile Elem* pro = NULL;
		while(tek!= NULL && tek->p != pp){
			pro = tek;
			tek = tek->sledeci;
		}
		if(tek != NULL){
			if(pro == NULL){
				prvi = prvi->sledeci;
				if(prvi == NULL) poslednji = NULL;
				delete (Elem*) tek;
			}else{
				pro->sledeci = tek->sledeci;
				if(tek->sledeci == NULL) poslednji = pro;
				delete (Elem*) tek;
			}
		}
	}

	unlock
}

int Lista::prazna() const{
	if(prvi == NULL) return 1;
	return 0;
}

PCB* Lista::izbaciPrvi() volatile{
	if(prvi == NULL) return NULL;
	lock
	PCB* pp = prvi->p;
	volatile Elem* tek = prvi;
	prvi = prvi->sledeci;
	if(prvi == NULL) poslednji = NULL;
	delete (Elem*)tek;
	unlock
	return pp;
}

void Lista::updateSleep() volatile {

	volatile Elem* tek = prvi;
	volatile Elem* pos = NULL;
	while(tek != NULL){
		tek->p->waitTime--;
		if(tek->p->waitTime  <= 0){
			volatile Elem* tmp = tek;
			if(tek == prvi){
				prvi = tek->sledeci;
				if(prvi == NULL) poslednji = NULL;
				pos = NULL;
			}else{
				pos->sledeci = tek->sledeci;
				if(tek == poslednji) poslednji = pos;
			}
			tek = tek->sledeci;
			tmp->p->blocked = 0;
			Scheduler::put((PCB*)tmp->p);
			delete (Elem*)tmp;
		}else{
			pos = tek;
			tek = tek->sledeci;
		}

		/*if(tek->p->waitTime <=0){
			tek->p->blocked = 0;
			Scheduler::put((PCB*)tek->p);
			PCB* tmp = tek->p;
			tek = tek->sledeci;
			izbaci(tmp);

		}else{
			tek = tek->sledeci;
		}*/

	}

}



void Lista::dodajUSleep(PCB* pp) volatile{
	volatile Elem* tek = prvi;
	volatile Elem* pro = NULL;
	volatile Elem* tmp = new Elem(pp);

	if(prvi == NULL){
		prvi = tmp;
	}else{
		while(tek != NULL && tek->p->waitTime < pp->waitTime){
			pp->waitTime -= tek->p->waitTime;
			pro = tek;
			tek = tek->sledeci;
		}
		if(pro == NULL){
			tmp->sledeci = (Elem*)prvi;
			prvi = tmp;
		}else{
			pro->sledeci = (Elem*)tmp;
			tmp->sledeci = (Elem*)tek;
		}
		if(tmp->sledeci!=NULL){
			tmp->sledeci->p->waitTime -= pp->waitTime;
		}
	}
}


void Lista::updateSleepFast() volatile{
	lock
	PCB* tmp;
	if(prvi != NULL){
		prvi->p->waitTime--;
	}
	while(prvi != NULL && prvi->p->waitTime <= 0){
		tmp = izbaciPrvi();
		if(tmp->blocked == 1){
			tmp->blocked = 0;
			Scheduler::put((PCB*)tmp);
			tmp->sem->waitQueue->izbaci(tmp);
			tmp->sem->value++;
		}
	}
	unlock
}







