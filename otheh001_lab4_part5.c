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

enum LockState{Init, idleLock, idleUnlock, holdButton, lockDoor} lockState;

void tick() {
    unsigned char x=PINA&0x01;
    unsigned char y=(PINA&0x02)>>1;
    unsigned char hash=(PINA&0x04)>>2;
    unsigned char lock=(PINA&0x80)>>7;
    static unsigned char lockStatus;
    static unsigned char sequence;
    static unsigned char button;
    switch (lockState) { //Transitions
        case Init:
	    lockState=idleLock;
	    sequence=0x00;
            break;
        case idleLock:
	    if (sequence==0x00) {
	        if (!x&&!y&&hash&&!lock) {
		    button=PINA;
		    lockState=holdButton;
		    sequence=0x01;
		}
		else {
		    sequence=0x00;
		}
	    }
            else if (sequence==0x01) {
                if (!x&&!y&&!hash&&!lock) {
		    sequence=0x01;
		}
		else if (x&&!y&&!hash&&!lock) {
		    button=PINA;
		    lockState=holdButton;
		    sequence=0x02;
		}
		else {
		    sequence=0x00;
		}
            }
	    else if (sequence==0x02) {
	        if (!x&&!y&&!hash&&!lock) {
                    sequence=0x02;
                }
                else if (!x&&y&&!hash&&!lock) {
		    button=PINA;
                    lockState=holdButton;
                    sequence=0x03;
                }
                else {
                    sequence=0x00;
                }
            }
    	    else if (sequence==0x03) {
		if (!x&&!y&&!hash&&!lock) {
		   sequence=0x03;
		}
		else if (x&&!y&&!hash&&!lock) {
		    lockState=idleUnlock;
		    lockStatus=0x01;
		    sequence=0x00;
		}
		else {
		    sequence=0;
		}
            }
	    break;
        case idleUnlock:
	    if (!x&&!y&&!hash&&lock) {
		sequence=0x00;
		lockStatus=0x00;
		lockState=lockDoor;
	    }
	    else if (sequence==0x00) {
                if (!x&&!y&&hash&&!lock) {
                    button=PINA;
                    lockState=holdButton;
                    sequence=0x01;
                }
                else {
                    sequence=0x00;
                }
            }
            else if (sequence==0x01) {
                if ((!x&&!y&&!hash&&!lock)) {
                    sequence=0x01;
                }
                else if (x&&!y&&!hash&&!lock) {
                    button=PINA;
                    lockState=holdButton;
                    sequence=0x02;
                }
                else {
                    sequence=0x00;
                }
            }
            else if (sequence==0x02) {
                if (!x&&!y&&!hash&&!lock) {
                    sequence=0x02;
                }
                else if (!x&&y&&!hash&&!lock) {
                    button=PINA;
                    lockState=holdButton;
                    sequence=0x03;
                }
                else {
                    sequence=0x00;
                }
            }
            else if (sequence==0x03) {
		if (!x&&!y&&!hash&&!lock) {
                   sequence=0x03;
                }
		else if (x&&!y&&!hash&&!lock) {
                    lockState=idleLock;
                    lockStatus=0x00;
                    sequence=0x00;
                }
                else {
                    sequence=0x00;
                }
            }
	    break;
	case holdButton:
	    if (button==PINA) {
		lockState=holdButton;
	    }
	    else if (!x&&!y&&!hash&&!lock) {
		lockState=!lockStatus?idleLock:idleUnlock;
	    }
	    else {
		//sequence=0x00;
	    }
	    break;
	case lockDoor:
     	    lockState=(!x&&!y&&!hash&&lock)?lockDoor:idleLock;
	    break;
	default:
	    lockState=Init;
    }
    PORTC=lockState;
    PORTB=lockStatus;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA=0x00; PORTA=0xFF;
    DDRB=0x00; PORTB=0xFF;
    DDRC=0xFF; PORTC=0x00;
    /* Insert your solution below */
    lockState=Init;
    while (1) {
	tick();
    }
    return 1;
}
