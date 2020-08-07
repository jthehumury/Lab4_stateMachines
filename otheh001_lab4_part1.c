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

enum LedState{Init, buttonReleaseLed1, buttonPressLed2, buttonReleaseLed2, buttonPressLed1} ledState;

void tick() {
    unsigned char button=PINA&0x01;
    static unsigned char leds;
    switch (ledState) { //Transitions
	case Init:
	    ledState=buttonReleaseLed1;
	    leds=0x01;
	    break;
	case buttonReleaseLed1:
	    ledState=button?buttonPressLed2:buttonReleaseLed1;
	    break;
	case buttonPressLed2:
	    ledState=button?buttonPressLed2:buttonReleaseLed2;
	    break;
	case buttonReleaseLed2:
	    ledState=button?buttonPressLed1:buttonReleaseLed2;
	    break;
	default:
	    ledState=Init;
    }
    switch (ledState) { //Actions
	case buttonPressLed2:
	    leds=0x02;
	    break;
	case buttonPressLed1:
	    leds=0x01;
    }
    PORTB=leds;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA=0x00; PORTA=0xFF;
    DDRB=0xFF; PORTB=0x00;
    /* Insert your solution below */
    ledState=Init;
    while (1) {
	tick();
    }
    return 1;
}
