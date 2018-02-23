/*
 * Jezgro.cpp
 *
 *  Created on: May 31, 2016
 *      Author: OS1
 */

#include"Jezgro.h"
#include "SCHEDULE.h"
#include <stdlib.h>
#include <iostream.h>
#include "Covece.h"
#include "Lista.h"

volatile int Jezgro::Zahtevana_promena_konteksta = 0;
volatile int Jezgro::lockFlag = 1;
volatile int Jezgro::brojac = 20;
volatile PCB* Jezgro::glavni = NULL;
volatile PCB* Jezgro::idle = NULL;

unsigned ttsp;
unsigned ttss;
unsigned ttbp;
unsigned oldTimerOFFF;
unsigned oldTimerSEGG;


void interrupt timerInterrupt(){
	//if(!Jezgro::Zahtevana_promena_konteksta) KernelSem::sleepQueue->updateSleep();
	if(!Jezgro::Zahtevana_promena_konteksta) KernelSem::sleepQueue->updateSleepFast();
	if (!Jezgro::Zahtevana_promena_konteksta && Jezgro::brojac >= 0) Jezgro::brojac--;
		if (Jezgro::brojac == 0 || Jezgro::Zahtevana_promena_konteksta) {
			if(Jezgro::lockFlag){
				Jezgro::Zahtevana_promena_konteksta = 0;
				asm {
					// cuva sp
					mov ttsp, sp
					mov ttss, ss
					mov ttbp, bp
				}

				PCB::running->sp = ttsp;
				PCB::running->ss = ttss;
				PCB::running->bp = ttbp;

				/*Jezgro::lockFlag = 0;
				cout << "Promena, brojac = " << Jezgro::brojac <<endl;
				asm cli;
				Jezgro::lockFlag = 1;*/


				if(!PCB::running->zavrsio && PCB::running->blocked == 0) Scheduler::put((PCB*)PCB::running);
				PCB* jad = Scheduler::get();
				if(jad == NULL){
					PCB::running = (PCB*)Jezgro::idle;
				}else{
					PCB::running = jad;
				}


				ttsp = PCB::running->sp;
				ttss = PCB::running->ss;
				ttbp = PCB::running->bp;

				Jezgro::brojac = PCB::running->kvant;

				asm {
					mov sp, ttsp   // restore sp
					mov ss, ttss
					mov bp, ttbp
				}
			}else{
				Jezgro::Zahtevana_promena_konteksta = 1;
			}
		}

		// poziv stare prekidne rutine koja se
	     // nalazila na 08h, a sad je na 60h
	     // poziva se samo kada nije zahtevana promena
	     // konteksta – tako se da se stara
	     // rutina poziva samo kada je stvarno doslo do prekida
	if(!Jezgro::Zahtevana_promena_konteksta) asm int 60h;

		//zahtevana_promena_konteksta = 0;
	//tick();
}



void Jezgro::inicTimer(){
	//unsigned oldTimerOFF, oldTimerSEG;
	asm{
		cli
		push es
		push ax

		mov ax,0   //  ; inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h //; pamti staru rutinu
		mov word ptr oldTimerSEGG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFFF, ax

		mov word ptr es:0022h, seg timerInterrupt	 //postavlja
		mov word ptr es:0020h, offset timerInterrupt //novu rutinu

		mov ax, oldTimerSEGG	 //	postavlja staru rutinu
		mov word ptr es:0182h, ax //; na int 60h
		mov ax, oldTimerOFFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
}

void Jezgro::restoreTimer(){
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEGG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
}

