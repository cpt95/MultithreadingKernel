/*
 * Lista.h
 *
 *  Created on: May 30, 2016
 *      Author: OS1
 */

#ifndef LISTA_H_
#define LISTA_H_

#include "PCB.h"

struct Elem{
	PCB* p;
	Elem* sledeci;
	Elem(PCB*);
};

class Lista{
	volatile Elem* prvi;
	volatile Elem* poslednji;
public:
	void updateSleep() volatile;
	Lista();
	void pisi() volatile;
	void dodaj (PCB*) volatile;
	PCB* izbaciPrvi() volatile;
	PCB* dohvatiPoID();
	void izbaci(PCB*) volatile;
	int prazna() const;

	void dodajUSleep(PCB*) volatile;
	void izbaciIzSleep(PCB*) volatile;
	void updateSleepFast() volatile;

};


#endif /* LISTA_H_ */
