/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum State{idle, incButtonHold, decButtonHold} state;

void tick() {
    unsigned char inc=PINA&0x01;
    unsigned char dec=(PINA&0x02)>>1;
    static unsigned char val=0x07;
    switch (state) {
	case idle:
	    if (!inc&&!dec) {
		state=idle;
	    }
	    else if (inc&&!dec) {
		if (val<9) {
                    val++;
                }
	        state=incButtonHold;
	    }
	    else if (!inc&&dec) {
		if (val>0) {
                    val--;
                }
		state=decButtonHold;
	    }
	    else if (inc&&dec) {
		state=idle;
		val=0x00;
	    }
	    break;
	case incButtonHold:
	    if (inc&&!dec) {
		state=incButtonHold;
	    }
	    else if (!inc&&!dec) {
		state=idle;
	    }
	    else if (dec) {
		state=idle;
    		val=0x00;		
	    }
	    break;
	case decButtonHold:
            if (!inc&&dec) {
                state=decButtonHold;
            }
            else if (!inc&&!dec) {
                state=idle;
            }
            else if (inc) {
                state=idle;
		val=0x00;
            }
	    break;
	default:
	    state=idle;
    }
    PORTC=val;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA=0x00; PORTA=0xFF;
    DDRC=0xFF; PORTC=0x00;
    /* Insert your solution below */
    state=Init;
    while (1) {
	tick();
    }
    return 1;
}
