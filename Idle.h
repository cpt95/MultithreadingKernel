/*
 * Idle.h
 *
 *  Created on: May 31, 2016
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_

#include"Thread.h"

class Idle : Thread{
public:
	Idle(int size, int time);
	void run();
};


#endif /* IDLE_H_ */
