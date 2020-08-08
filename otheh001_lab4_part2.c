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

enum State{Init, idle, incButtonHold, decButtonHold, reset} state;

void tick() {
    unsigned char inc=PINA&0x01;
    unsigned char dec=(PINA&0x02)>>1;
    static unsigned char val;
    switch (state) {
	case Init:
	    state=idle;
	    val=0x07;
	    break;
	case wait:
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
		state=reset;
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
		state=reset;	    
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
                state=reset;
            }
	    break;
	case reset:
	    val=0x00;
	    state=idle;
	    break;
	default:
	    state=Init;
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
