/*
 * Semaphor.cpp
 *
 *  Created on: Jun 3, 2016
 *      Author: OS1
 */

#include "Semaphor.h"
#include <iostream.h>
#include "Covece.h"

int Semaphore::val() const {
	return myImpl->val();
}

Semaphore::Semaphore(int init){
	myImpl = new KernelSem(init);
}
Semaphore::~Semaphore(){
	delete myImpl;
}

int Semaphore::wait(unsigned int time){
	return myImpl->wait(time);
}

void Semaphore::signal(){
	myImpl->signal();
}
