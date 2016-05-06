#include <dos.h>
#include <conio.h>
#include <stdio.h>

#define waitingForKeyboardInputBuffer (inp(0x64) & 0x02) != 0x00
#define resetTheInterruptController outp(0x20, 0x20)  
#define ESCAPE 0x01
#define FALSE 0
#define TRUE 1
#define confirmationCode 0xFA
#define CAPS_LOCK 0x04
#define NUM_LOCK 0x02
#define CAPS_AND_NUM_LOCK 0x6
#define TURNOFF 0x00

void interrupt far newKeyboardInterrupt(...);
void interrupt far(*oldKeyboardInterrupt)(...);

void indicatorController(unsigned char mask);
void blinking (void);

int exit = FALSE;

void main(){
	oldKeyboardInterrupt = getvect(9);	
	setvect(9, newKeyboardInterrupt);		
	while(!exit){
			blinking();
	}		
	setvect(9, oldKeyboardInterrupt);
	return;
}

void interrupt far newKeyboardInterrupt(...){
	unsigned char scanCode = 0;
	oldKeyboardInterrupt();
	
	scanCode = inp(0x60);
	if(scanCode == ESCAPE){
		exit = TRUE;
	}
	
	printf("\n\nScan code: %x", scanCode);
	resetTheInterruptController;
}

void indicatorController(unsigned char mask){
	transferFailed = TRUE;
	
	while (transferFailed) {
		while(waitingForKeyboardInputBuffer);
		outp(0x60, 0xED);
		delay(50);
	}
	
	transferFailed = TRUE;
	
	while (transferFailed) {
		while(waitingForKeyboardInputBuffer);
		outp(0x60, mask);
		delay(50);
	}
}

void blinking ()
{
	indicatorController(TURNOFF);
    delay(50);
	
	indicatorController(NUM_LOCK);
	delay(150);
	
	indicatorController(TURNOFF);
    delay(50);
	
	indicatorController(CAPS_LOCK);
	delay(150);
	
	indicatorController(TURNOFF);
    delay(50);
	
	indicatorController(CAPS_AND_NUM_LOCK);
	delay(150);
	
	indicatorController(TURNOFF);
    delay(50);
}
