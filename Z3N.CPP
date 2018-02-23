//pretpostavljeni memorijski model: huge

#include <iostream.h>
#include <dos.h>
#include "schedule.h"
#include <assert.h>
#include <stdlib.h>
#include "Thread.h"
#include "PCB.h"
#include "Idle.h"
#include "Lista.h"
#include "Jezgro.h"
#include "bounded.h"
#include "intLock.h"
#include "IVTEntry.h"
#include "Dogadjaj.h"
#include "event.h"



//class BoundedBuffer;
Time TIME_SLICE = 0;   // 0 ili defaultTimeSlice

int N = 3;            // 1 <= N <= 19

//---------------------------------------------------------------------------//

volatile int theEnd=0;



char keymap[128] = {
  0 , 27,'1','2','3','4','5','6','7','8','9','0','-','=', 8 , 9,
 'q','w','e','r','t','y','u','i','o','p','[',']', 13, 0 ,'a','s',
 'd','f','g','h','j','k','l',';',0,0,'`','\\','z','x','c','v','b',
 'n','m',',','.','/', 0 ,'*', 0 ,' '
};

class KeyboardEvent :public Thread{
public:

	KeyboardEvent (BoundedBuffer*);
	virtual ~KeyboardEvent () {waitToComplete();}
	Thread* clone() const { return new KeyboardEvent(myBuffer); };
protected:

	virtual void run();

	char znak;

private:

	BoundedBuffer* myBuffer;
};

//---------------------------------------------------------------------------//
KeyboardEvent::KeyboardEvent(BoundedBuffer* bb) : Thread(), myBuffer(bb)
{
	theEnd = 0;
}



void KeyboardEvent::run()
{
	intLock
	Event event9(9);
	cout<<"KeyebordListener started!"<<endl;
	intUnlock
	char scancode, status, znak;

	while (!theEnd) {

		event9.wait();
		do{
			status = inportb(0x64); // ocitava statusni reg. sa 64h


			if (status & 0x01){           // Can I read?
				scancode = inportb(0x60);

				if (scancode==-127){
					theEnd = 1;
					myBuffer->append('!'); //finished
				}else {
					if (scancode&0x80) {
						myBuffer->append(keymap[scancode&0x7F]);
					}
				}

			};
       //////////////////////
			asm{
				cli
				in      al, 61h         //; Send acknowledgment without
				or      al, 10000000b   //;   modifying the other bits.
				out     61h, al         //;
				and     al, 01111111b   //;
				out     61h, al         //;
				mov     al, 20h         //; Send End-of-Interrupt signal
				out     20h, al         //;
				sti
			}

		 }while (!theEnd && status & 0x01); //dok se ocitava takav status da je pritisnut neki taster

	}// while

	intLock
		cout<<endl<<"KeyebordListener stopped!"<<endl;
	intUnlock

}



class Producer : public Thread {
public:
	Producer (BoundedBuffer* bb, char y, Time time_slice);
	virtual ~Producer() {waitToComplete(); }
   // Thread* clone() const { return new Producer(myBuffer, x, time_slice_clone); }
protected:
	virtual void run ();
	char produce() {return x;}; // Produce an item

private:
	Time time_slice_clone;
	BoundedBuffer* myBuffer;
	char x;
	Semaphore sleep;
};



//---------------------------------------------------------------------------//
class Consumer : public Thread {
public:
	Consumer (BoundedBuffer* bb) : Thread(defaultStackSize, 0), myBuffer(bb), sleep(0) {}
	virtual ~Consumer() {waitToComplete(); }
	//Thread* clone() const { return new Consumer(myBuffer); }
protected:
	virtual void run ();
	void consume(char p); // Consume an item

private:
	BoundedBuffer* myBuffer;
	Semaphore sleep;
};




//---------------------------------------------------------------------------//
// Korisnicki program mora obavezno da definise ovu f-ju
//---------------------------------------------------------------------------//
//void tick(){
//}

//---------------------------------------------------------------------------//


Producer::Producer (BoundedBuffer* bb, char y, Time time_slice)
: Thread(defaultStackSize, time_slice),myBuffer(bb), x(y), sleep(0), time_slice_clone(time_slice) {}

	void Producer::run () {
	while(!theEnd) {
		char d = produce();
		myBuffer->append(d);
		assert(1 != sleep.wait(10));
	}
}

//---------------------------------------------------------------------------//


void Consumer::consume(char p) {
	intLock
	cout<<p<<" ";
	intUnlock
} // Consume an item

void Consumer::run () {

	int i = 0;
	while(!theEnd) {
		char d = myBuffer->take();
		consume(d);
		if (i++ == 40) {
			assert(1 != sleep.wait(5));
			i = 0;
		}else for(int j=0;j<200;j++);
	}

	intLock
	cout<<endl<<"ESC pressed - empty the buffer!"<<endl;
	intUnlock

	while (myBuffer->fullCount()){
		char d = myBuffer->take();
		consume(d);
		dispatch();
	}


	intLock
	cout<<endl<<"Happy End"<<endl;
	intUnlock
}


void doSomething(){
	lock
	new PCB(20);
	new Idle(4096, 2);
	BoundedBuffer* buff = new BoundedBuffer(250);
	Producer **pro = new Producer*[N];
	KeyboardEvent* kev;
	int i;

	Consumer* con;
	con = new Consumer(buff);
	con->start();

	for (i=0; i<N; i++){
		pro[i] = new Producer(buff,'0'+i, TIME_SLICE);
		pro[i]->start();
	}

	kev = new KeyboardEvent(buff);
	unlock
	kev->start();

	for (int j=0; j<N; j++){
		delete pro[j];
	}
	delete [] pro;
	delete kev;
	delete con;
	delete buff;

	cout<<"Happy End"<< sizeof(unsigned int) << endl;
}



int main(){
	
  Jezgro::inicTimer();

  doSomething();

  Jezgro::restoreTimer();
 
  return 0;
}

