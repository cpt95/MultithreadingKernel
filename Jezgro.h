/*
 * Jezgro.h
 *
 *  Created on: May 31, 2016
 *      Author: OS1
 */

#ifndef JEZGRO_H_
#define JEZGRO_H_
#include"PCB.h"

void interrupt timerInterrupt();

class Jezgro{
public:
	static volatile int Zahtevana_promena_konteksta;
	static volatile int lockFlag;
	static volatile int brojac;
	static volatile PCB* glavni;
	static volatile PCB* idle;


	static void inicTimer();
	static void restoreTimer();
};



#endif /* JEZGRO_H_ */
