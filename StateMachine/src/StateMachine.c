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
	STATE_A,
	STATE_B,
	STATE_C,
	STATE_D,
	STATE_LAST,
}StateType_t;

typedef struct{
	StateType_t State;
	void (*func)(void);
}StateMachine_t;

StateType_t StateCurrent=STATE_A;
StateType_t StateNext=STATE_A;
int DelayTiming=0;
int CurrentTime=0;

void State_A(void);
void State_B(void);
void State_C(void);
void State_D(void);
void StateDelay(void);

void State_A(void){
	StateCurrent = STATE_A;
	printf("!!!State A\n");

	DelayTiming=2;
	StateNext=STATE_B;
}

void State_B(void){
	StateCurrent = STATE_B;
	printf("!!!State B\n");

	DelayTiming=5;
	StateNext=STATE_C;
}

void State_C(void){
	StateCurrent = STATE_C;
	printf("!!!State C\n");

	DelayTiming=3;
	StateNext=STATE_D;
}

void State_D(void){
	StateCurrent = STATE_D;
	printf("!!!State D\n");

	DelayTiming=8;
	StateNext=STATE_A;
}

void StateDelay(void){
	if(DelayTiming!=0){
		if(CurrentTime+DelayTiming>(clock()/CLOCKS_PER_SEC))
			return;
	}

	printf("Different Time = %d\n",(clock()/CLOCKS_PER_SEC)-CurrentTime);
	DelayTiming=0;
	StateCurrent = StateNext;
}

StateMachine_t StateMachine[]={
		{ STATE_A, State_A },
		{ STATE_B, State_B },
		{ STATE_C, State_C },
		{ STATE_D, State_D },
};

void StateRun(void){
	if(StateCurrent<STATE_LAST && DelayTiming==0){
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
