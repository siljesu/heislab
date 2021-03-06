/**
 * @file
 * @brief Main file of the elevator. FSM module implementation
**/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include "elevator.h"

#define DOORS_OPEN_TIME 3000

static void sigint_handler(int sig){
    (void)(sig);
    printf("Terminating elevator\n");
    void elevator_stopMotor();
    exit(0);
}

State s_idle();
State s_move();
State s_handleOrder();
State s_emergencyStop();
State s_doorsOpenTimer();


State s_idle(){
    
    elevator_setRelativePosition(g_currentMoveDirection);
    elevator_checkAndAddOrder(g_FLOOR, g_currentMoveDirection);

    if (elevator_checkForStop()){
        return EMERGENCY_STOP;
    }
    
    if (p_firstOrder->floor < g_FLOOR && p_firstOrder->activeOrder){
        elevator_goDown();
        return MOVE;
    }
    
    if (p_firstOrder->floor > g_FLOOR && p_firstOrder->activeOrder){
        elevator_goUp();
        return MOVE;
    }
    
    if (p_firstOrder->floor == g_FLOOR && p_firstOrder->activeOrder){
        switch (g_relativePosition){
        case BELOW:
            elevator_goUp();
            return MOVE;
            break;
        case ABOVE:
            elevator_goDown();
            return MOVE;
            break;
        case AT:
            elevator_stopMotor();
            return HANDLE_ORDER;
            break;
        }
    }
    return IDLE;
}

State s_move(){

    elevator_setRelativePosition(g_currentMoveDirection);
    elevator_checkAndAddOrder(g_FLOOR, g_currentMoveDirection);

    g_FLOOR = elevator_findCurrentFloor(g_FLOOR);
    
    if (elevator_checkForStop()){
        return EMERGENCY_STOP;
    }
    
    if (elevator_amIAtFloor(p_firstOrder->floor)){
        elevator_stopMotor();
        return HANDLE_ORDER;
    }
    
    return IDLE;
}

State s_doorsOpenTimer(){

    time_t startTime = clock() * 1000 / CLOCKS_PER_SEC;

    while (startTime + DOORS_OPEN_TIME >= clock() * 1000 / CLOCKS_PER_SEC){
        elevator_checkAndAddOrder(g_FLOOR, g_currentMoveDirection);

        if (elevator_checkForStop()){
            return EMERGENCY_STOP;
        }
        if (elevator_obstruction()){ 
            return DOORS_OPEN_TIMER;
        }

        if (elevator_ordersAtThisFloor()) {
            return HANDLE_ORDER;
        }
    }

    elevator_closeDoors();
    return IDLE;
}

State s_handleOrder(){

    if (elevator_checkForStop()){
            return EMERGENCY_STOP;
        }

    elevator_handleOrder(); 
    
    elevator_openDoors();
    
    return DOORS_OPEN_TIMER;
}

State s_emergencyStop(){

    elevator_stopLightOn();
    elevator_clearAllOrders();

    if (!elevator_stopSignal() && elevator_amIAtAnyFloor()){
        elevator_openDoors();
        elevator_stopLightOff();
        return DOORS_OPEN_TIMER;
    }
    if (!elevator_checkForStop())
    {
        elevator_stopLightOff();
        return IDLE;
    }
    return EMERGENCY_STOP;
}


int main()
{

    int error = hardware_init();

    if (error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    signal(SIGINT, sigint_handler);

    elevator_init();

    while (!elevator_amIAtAnyFloor()){
        //Waiting 'till a floor is reached
    }

    elevator_stopMotor();
    StateFunction p_state = s_idle;

    while (1){
        State nextState = p_state();
        switch (nextState){
        case IDLE:
            p_state = s_idle;
            break;
        case MOVE:
            p_state = s_move;
            break;
        case DOORS_OPEN_TIMER:
            p_state = s_doorsOpenTimer;
            break;
        case EMERGENCY_STOP:
            p_state = s_emergencyStop;
            break;
        case HANDLE_ORDER:
            p_state = s_handleOrder;
            break;
        }
    }
}