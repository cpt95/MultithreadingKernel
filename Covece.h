/*
 * Covece.h
 *
 *  Created on: Jun 3, 2016
 *      Author: OS1
 */

#ifndef COVECE_H_
#define COVECE_H_

#include "Lista.h"

class KernelSem{
public:
	static volatile Lista* sleepQueue;
	int value;
	Lista* waitQueue;

	KernelSem(int);
	~KernelSem();

	int wait(int);
	void signal();
	int val() const;
};



#endif /* COVECE_H_ */
