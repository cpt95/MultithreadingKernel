/*
 * event.cpp
 *
 *  Created on: Jun 4, 2016
 *      Author: OS1
 */

#include "event.h"
#include "Dogadjaji.h"
#include "IVTEntry.h"

Event::Event(IVTNo ivtNo){
	//myImpl = IVTEntry::Key->ke;
	//IVTEntry::Key->inicijalizacija();
	myImpl = new KernelEv(ivtNo);
	IVTEntry::dohvatiIVT(ivtNo)->inicijalizacija(myImpl);

}

Event::~Event(){
	//IVTEntry::Key->restore();
	lock
	delete myImpl;
	unlock
}

void Event::wait(){
	myImpl->wait();
}
