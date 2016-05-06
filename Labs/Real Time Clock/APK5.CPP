#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <io.h>
#include <ctype.h>
#include <string.h>

#define SELECT_SECONDS_REGISTER outp(0x70,0x00) 
#define SELECT_MINUTES_REGISTER outp(0x70,0x02)
#define SELECT_HOURS_REGISTER outp(0x70,0x04)

void setTime();
void getTime();
void setAlarmClock();
void turnOffAlarmClock();
unsigned char decimalToBCD (int);

int count = 0;

void interrupt far(*oldDelay)(...);
void interrupt far newDelay(...){
	count++;
	outp(0x20,0x20);
	outp(0xA0,0x20);
}

void interrupt(*oldAlarmClock) (...);
void interrupt newAlarmClock (...){
	write(1,"BEEP BEEP BEEP BEEP",19);
	oldAlarmClock();
	turnOffAlarmClock();
}

int main(){
	while(1){
		clrscr(); 
		printf("\n1 - Set time.\n");
		printf("\n2 - Get time.\n");
		printf("\n3 - Set alarm clock.\n");
		printf("\n4 - Delay.\n");
		printf("\n0 - Exit.\n");
		printf("\nSelect: ");

		switch(getch()){

			case '1':{
				clrscr(); 
				setTime();
				printf("\nPress any key to continue...");
				getch();
			}break;

			case '2':{
				clrscr(); 
				getTime();
				printf("\nPress any key to continue...");
				getch();
			}break;

			case '3':{
				clrscr();
				setAlarmClock();
				printf("\nPress any key to continue...");
				getch();
			}break;

			case '4':{
				clrscr();
				int delayTime;
				printf("\nEnter delay time: ");
				scanf("%d",&delayTime);
				getTime();
				delay(delayTime);
				getTime();
				printf("\nPress any key to continue...");
				getch();
			}break;
			case '0':{
				clrscr();
				return 0;
			}break;
		}
	}
}

void setTime(){
	int result;
	outp(0x70,0xA);
	for(int i = 0; i < 256; i++){
		result = inp(0x71);
		if((result % 2) == 0){
			int hours,minutes,seconds;
			
			printf("\nEnter hours: ");
			scanf("%d",&hours);
			
			printf("\nEnter minutes: ");
			scanf("%d",&minutes);
			
			printf("\nEnter seconds: ");
			scanf("%d",&seconds);
			
			outp(0x70,0xB);
			outp(0x71,inp(0x71) | 0x80);
			
			SELECT_SECONDS_REGISTER;
			outp(0x71,decimalToBCD(seconds));
			
			SELECT_MINUTES_REGISTER;
			outp(0x71,decimalToBCD(minutes));
			
			SELECT_HOURS_REGISTER;
			outp(0x71,decimalToBCD(hours));
			
			getTime();
			
			outp(0x70,0xB);
			outp(0x71, inp(0x71) & 0x7F);
			break;
		}
	}
}

void getTime(){
	int seconds;
	int minutes;
	int hours;
	
	SELECT_SECONDS_REGISTER;
	seconds = inp(0x71);

	SELECT_MINUTES_REGISTER;
	minutes = inp(0x71);

	SELECT_HOURS_REGISTER;
	hours = inp(0x71);

	printf("Current time: %x : %x : %x\n",hours,minutes,seconds);
}

void setAlarmClock(){
	int hours,minutes,seconds;
	
	printf("\nEnter the alarm time: ");
	scanf("%d %d %d",&hours,&minutes,&seconds);
	
	disable();
	
	outp(0x70, 0x0A);
	int i = 0;
	
	do{
        outp(0x70, 0x0A);
		i++;
		if(i == 256)
			break;
    } while( inp(0x71) & 0x80 ); 
	
	if(i == 256){
		return;
	}
	
	outp(0x70, 0x05);
	outp(0x71, decimalToBCD(hours));
	
	outp(0x70, 0x03);
	outp(0x71, decimalToBCD(minutes));
	
	outp(0x70, 0x01);
	outp(0x71, decimalToBCD(seconds));
	
	outp(0x70, 0xB);
	outp(0x71, (inp(0x71) | 0x20));
	
	oldAlarmClock = getvect(0x4A);
	setvect(0x4A,newAlarmClock);
	outp(0xA1, (inp(0xA0) & 0xFE));
	
	enable();
	
	printf("Alarm is set to: %d:%d:%d",hours,minutes,seconds);
}

void turnOffAlarmClock(){
	if (oldAlarmClock == NULL) {
		return;
	}

	disable();

	setvect(0x4A, oldAlarmClock);
	outp(0xA1, (inp(0xA0) | 0x01));
	
	int i = 0;
	outp(0x70, 0x0A);
	
	do{
        outp(0x70, 0x0A);
		i++;
		if(i == 256)
			break;
    } while( inp(0x71) & 0x80 ); 
	
	if(i == 256){
		return;
	}

	outp(0x70, 0x05);
	outp(0x71, 0x00);

	outp(0x70, 0x03);
	outp(0x71, 0x00);

	outp(0x70, 0x01);
	outp(0x71, 0x00);

	outp(0x70, 0xB);
	outp(0x71, (inp(0x71) & 0xDF));

	enable();
}

void delay(int delayTime){
	disable();
    oldDelay = getvect(0x70);
    setvect(0x70, newDelay);
    enable();

    outp(0xA1,inp(0xA1) & 0xFE);

    outp(0x70, 0xB);
    outp(0x71, inp(0x71) | 0x40);

    count = 0;
    while(count != delayTime);
	
    setvect(0x70, oldDelay);
}

unsigned char decimalToBCD (int decimal){
    return (unsigned char)(((decimal / 10) << 4)|(decimal % 10));
}



