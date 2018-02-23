/*
 * Dogadjaj.h
 *
 *  Created on: Jun 4, 2016
 *      Author: OS1
 */

#ifndef DOGADJAJ_H_
#define DOGADJAJ_H_

#include "PCB.h"
#include "event.h"

class KernelEv{
public:
	PCB* myPCB;
	IVTNo ulaz;
	KernelEv(IVTNo);
	~KernelEv();

	void wait();
	void signal();
};



#endif /* DOGADJAJ_H_ */
