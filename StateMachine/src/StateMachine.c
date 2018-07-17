/*
 ============================================================================
 Name        : State.c
 Author      : gsahin
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "time.h"

typedef enum{
	FALSE=0,
	TRUE=!FALSE,
}State_t;

#define __IS_NEUTRAL_POSITION() (TRUE)
#define __MAX_PERCENT(A,B)	(A*(1+B/100.0))
#define __MIN_PERCENT(A,B)	(A*(1-(B/100.0)))

#define __IS_BETWEEN_TOLERANCE(A,B,C) ((A<=__MAX_PERCENT(B,C))?((A>=__MIN_PERCENT(B,C)?TRUE:FALSE)):FALSE)

typedef enum{
	STATE_NEUTRAL,
	STATE_NORTH,
	STATE_SOUTH,
	STATE_WEST,
	STATE_EAST,
	STATE_NORTH_WEST,
	STATE_NORTH_EAST,
	STATE_SOUTH_WEST,
	STATE_SOUTH_EAST,
	STATE_LAST,
}StateType_t;

typedef enum{
	JOYSTICK_TWO_AXIS,
	JOYSTICK_ONE_AXIS_NORTH_SOUTH,
	JOYSTICK_ONE_AXIS_WEST_EAST,
	JOYSTICK_LAST,
}JoystickAxisType_t;

typedef enum{
	ANALOG_8_32V,
	ANALOG_RATIOMETRIC,
	ANALOG_URRENT_OUT,
	CAN_J1939,
	CAN_OPEN,
}JoystickBaseType_t;

typedef struct{
	StateType_t State;
	void (*func)(void);
}StateMachine_t;

StateType_t StateCurrent=STATE_NEUTRAL;
StateType_t StateNext=STATE_NEUTRAL;
int DelayTiming=0;
int CurrentTime=0;
int JoystickAxisType=JOYSTICK_TWO_AXIS;
int JosytickBaseType = CAN_J1939;

void StateNeutral(void);
void StateNorth(void);
void StateSouth(void);
void StateEast(void);
void StateNorthWest(void);
void StateNorthEast(void);
void StateSouthWest(void);
void StateSouthEast(void);
void StateDelay(void);

typedef struct{
	struct{
		int reel;
		int comp;
	}x_axis,y_axis;
}JoystickAxisValue_t;

JoystickAxisValue_t JoystickAxis;

void StateNeutral(void){
	JoystickAxisValue_t axis;

	StateCurrent = STATE_NEUTRAL;

	if(__IS_NEUTRAL_POSITION() == FALSE){
		return;
	}

	// disable interrupt
	axis=JoystickAxis;
	int tol=__IS_BETWEEN_TOLERANCE(4950,5000,1);
	printf("!!!State A = %d \n",tol);
	// enable interrupt

	//axis check

	if (JoystickAxisType == JOYSTICK_TWO_AXIS) {

		switch (JosytickBaseType) {
		case CAN_J1939:

			printf("!!!State A\n");

			break;
		default:
			break;
		}
	}

	DelayTiming=2;
	StateNext=STATE_NORTH;
}

void StateNorth(void){
	StateCurrent = STATE_NORTH;
	printf("!!!State B\n");

	DelayTiming=5;
	StateNext=STATE_SOUTH;
}

void StateSouth(void){
	StateCurrent = STATE_SOUTH;
	printf("!!!State C\n");

	DelayTiming=3;
	StateNext=STATE_WEST;
}

void StateEast(void){
	StateCurrent = STATE_WEST;
	printf("!!!State D\n");

	DelayTiming=8;
	StateNext=STATE_NEUTRAL;
}

void StateDelay(void){
	if(DelayTiming!=0 && CurrentTime+DelayTiming>(clock()/CLOCKS_PER_SEC)){
		return;
	}

	printf("Different Time = %d\n",(int)(clock()/CLOCKS_PER_SEC)-CurrentTime);
	DelayTiming=0;
	StateCurrent = StateNext;
}

StateMachine_t StateMachine[]={
		{ STATE_NEUTRAL, StateNeutral },
		{ STATE_NORTH, StateNorth },
		{ STATE_SOUTH, StateSouth },
		{ STATE_WEST, StateEast },
};

void StateRun(void){
	if(StateCurrent<STATE_NORTH_WEST && DelayTiming==0){
		(*StateMachine[StateCurrent].func)();
		CurrentTime=clock()/CLOCKS_PER_SEC;
	}else{
		StateDelay();
	}
}
int main(void) {
	while(1){
		StateRun();
	}
}
