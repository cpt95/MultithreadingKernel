/*
 * IVTEntry.cpp
 *
 *  Created on: Jun 4, 2016
 *      Author: OS1
 */

#include "IVTEntry.h"
#include<stdlib.h>
#include "Dogadjaj.h"
#include"PCB.h"
#include <iostream.h>





IVTEntry** IVTEntry::ivt = new IVTEntry*[257];
//IVTEntry* IVTEntry::Key = new IVTEntry(9);
unsigned pomocniOFF;
unsigned pomocniSEG;


IVTEntry* entry9 = new IVTEntry(9);
void interrupt keyInterrupt(){
	//ispisi(cout <<"U prekidnoj rutini" << endl)
	entry9->signal();
	dispatch();
}

IVTEntry::IVTEntry(IVTNo ulazz){
	//ke = new KernelEv();
	this->ulaz = ulazz;
	IVTEntry::ivt[ulaz] = this;
	//IVTEntry::Key = this;
}


IVTEntry* IVTEntry::dohvatiIVT(IVTNo index){
	return IVTEntry::ivt[index];
}

void IVTEntry::signal(){
	this->ke->signal();
}



void IVTEntry::inicijalizacija(KernelEv* ev){
	ke = ev;
	asm{
		cli
		push es
		push ax

		mov ax,0   //  ; inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0026h //; pamti staru rutinu
		mov word ptr pomocniSEG, ax
		mov ax, word ptr es:0024h
		mov word ptr pomocniOFF, ax

		mov word ptr es:0026h, seg keyInterrupt	 //postavlja
		mov word ptr es:0024h, offset keyInterrupt //novu rutinu


		pop ax
		pop es
		sti
	}
	stariOFF = pomocniOFF;
	stariSEG = pomocniSEG;
}


void IVTEntry::restore(){
	lock
	pomocniOFF = stariOFF;
	pomocniSEG = stariSEG;
	asm {
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr pomocniSEG
		mov word ptr es:0026h, ax
		mov ax, word ptr pomocniOFF
		mov word ptr es:0024h, ax

		pop ax
		pop es
	}
	unlock
}
