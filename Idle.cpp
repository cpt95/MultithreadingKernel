/*
 * Idle.cpp
 *
 *  Created on: May 31, 2016
 *      Author: OS1
 */

#include"Idle.h"
#include <iostream.h>
#include "Jezgro.h"
#include "SCHEDULE.h"
#include "PCB.h"
#include "Covece.h"


Idle::Idle(int size, int time):Thread(size, time){ }

void Idle::run(){
	while(1){
		//ispisi(cout << "U IDLE" << endl)
		//PCB::baraba->pisi();
		for (int j = 0; j< 30000; ++j)
			for (int k = 0; k < 30000; ++k);
	}
}



