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

enum State{Init, wait, incPress, incButtonHold, incRelease, decPress, decButtonHold, decRelease, pressOff, reset} state;

void tick() {
    unsigned char inc=PINA&0x01;
    unsigned char dec=(PINA&0x02)>>1;
    static unsigned char val;
    switch (state) {
	case Init:
	    state=wait;
	    val=0x07;
	    break;
	case wait:
	    if (!inc&&!dec) {
		state=wait;
	    }
	    else if (inc&&!dec) {
	        state=incPress;
	    }
	    else if (!inc&&dec) {
		state=decPress;
	    }
	    else if (inc&&dec) {
		state=reset;
	    }
	    break;
	case incPress:
            if (val<9) {
                val++;
            }
	    state=incButtonHold;
	    break;
	case incButtonHold:
	    if (inc&&!dec) {
		state=incButtonHold;
	    }
	    else if (!inc&&!dec) {
		state=incRelease;
	    }
	    else if (dec) {
		state=reset;	    
	    }
	    break;
	case incRelease:
	    if (!inc&&!dec) {
		state=incRelease;
	    }
	    else if (inc&&!dec) {
		state=pressOff;
	    }
	    else if (dec) {
		state=reset;
	    }
	    break;
	case decPress:
	    if (val>0) {
                val--;
            }
	    state=decButtonHold;
	    break;
	case decButtonHold:
            if (!inc&&dec) {
                state=decButtonHold;
            }
            else if (!inc&&!dec) {
                state=decRelease;
            }
            else if (inc) {
                state=reset;
            }
            break;
        case decRelease:
            if (!inc&&!dec) {
                state=decRelease;
            }
            else if (!inc&&dec) {
                state=pressOff;
            }
            else if (inc) {
                state=reset;
            }
	    break;
	case pressOff:
	    if (inc&&dec) {
		state=reset;
	    }
	    else if (inc||dec) {
		state=pressOff;
	    }
	    else {
		state=wait;
	    }
	    break; 
	case reset:
	    val=0x00;
	    state=wait;
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
