/*
 * IVTEntry.h
 *
 *  Created on: Jun 4, 2016
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_






#include "Dogadjaj.h"
#include <dos.h>
#include"event.h"
void interrupt keyInterrupt();

class IVTEntry{
public:
	KernelEv* ke;
	unsigned int stariOFF;
	unsigned int stariSEG;
	IVTNo ulaz;
	//static IVTEntry* Key;

	IVTEntry(IVTNo);
	~IVTEntry();

	void inicijalizacija(KernelEv*);
	void restore();
	void signal();

	static IVTEntry* dohvatiIVT(IVTNo);
private:
	static IVTEntry** ivt;
};



#endif /* IVTENTRY_H_ */
