/*
 * PCB.h
 *
 *  Created on: May 23, 2016
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "Thread.h"
#include "Lista.h"
#include "Jezgro.h"


#define ispisi(text) Jezgro::lockFlag = 0;\
	text ;\
	Jezgro::lockFlag = 1;\
	if(Jezgro::Zahtevana_promena_konteksta){\
		dispatch();\
	}


#define lock asm {\
	pushf;\
	cli;\
}

// Dozvoljava prekide
#define unlock asm popf
class Lista;
class KernelSem;

//void dispatch();
class PCB{
public:

	static PCB* running;
	static int idTrenutni;
	static Lista* baraba;
	int id;
	unsigned int sp;
	unsigned int ss;
	unsigned int bp;
	unsigned int* stack;
	int kvant;
	int zavrsio;
	int blocked;
	int waitTime;
	Thread* myThread;
	Lista* listaCekanja;
	KernelSem* sem;

	PCB(int size, int time, Thread* t);
	PCB(int time);
	//PCB(int size, int time);

	int dohvatiID() const;

	static void wrapper();

	int isFinished() const;
	~PCB();

};


#endif /* PCB_H_ */
