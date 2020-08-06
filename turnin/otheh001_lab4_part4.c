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

enum LockState{Init, idleLock, hashHold, hashRelease, yHold, idleUnlock, lockDoor} lockState;

void tick() {
    unsigned char x=PINA&0x01;
    unsigned char y=(PINA&0x02)>>1;
    unsigned char hash=(PINA&0x04)>>2;
    unsigned char lock=(PINA&0x80)>>7;
    static unsigned char lockStatus;
    switch (lockState) { //Transitions
	case Init:
	    lockState=idleLock;
	    break;
	case idleLock:
	    lockState=(!x&&!y&&hash&&!lock)?hashHold:idleLock;
	    break;
	case hashHold:
	    if (!x&&!y&&hash&&!lock) {
		lockState=hashHold;
	    }
	    else if (!x&&!y&&!hash&&!lock) {
		lockState=hashRelease;
	    }
	    else {
		lockState=!lockStatus?idleLock:idleUnlock;
	    }
	    break;
	case hashRelease:
	    if (!x&&!y&&!hash&&!lock) {
		lockState=hashRelease;
	    }
	    else if (!x&&y&&!hash&&!lock) {
		lockState=yHold;
	    }
	    else {
		lockState=!lockStatus?idleLock:idleUnlock;	
	    }
	    break;
	case yHold:
	    if (!x&&y&&!hash&&!lock) {
		lockState=yHold;
	    }
	    else if (!x&&!y&&!hash&&!lock) {
		lockState=lockStatus?idleUnlock:idleLock;
	    }
	    break;
	case idleUnlock:
	    if (!x&&!y&&!hash&&!lock) {
		lockState=idleUnlock;
	    }
	    else if (!x&&!y&&!hash&&lock) {
		lockState=lockDoor;
	    }
	    else if (!x&&!y&&hash&&!lock) {
		lockState=hashHold;
	    }
	    break;
	case lockDoor:
	    lockState=(!x&&!y&&!hash&&lock)?lockDoor:idleLock;
	    break;
	default:
	    lockState=Init;
    }
    switch (lockState) { //Actions
	case idleLock:
	    lockStatus=0x00;
	    break;
	case yHold:
	    lockStatus=!lockStatus?0x01:0x00;
	    break;
	case lockDoor:
	    lockStatus=0x00;
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
