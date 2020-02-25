#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include "elevator.h"

#define DOORS_OPEN_TIME 3000

int g_FLOOR;

HardwareMovement currentMoveDirection;

static void sigint_handler(int sig){
    (void)(sig);
    printf("Terminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}

void s_idle();
void s_move();
void s_handleOrder();
void s_emergencyStop();
void s_doorsOpenTimer();

void s_idle(){

    elevator_setRelativePosition(currentMoveDirection);
    elevator_checkAndAddOrder(g_FLOOR, currentMoveDirection);

    if (hardware_read_stop_signal() && elevator_amIAtAnyFloor()) {
        elevator_stopMotor();
        elevator_openDoors();
        return EMERGENCY_STOP;
    }
    if (hardware_read_stop_signal() && !elevator_amIAtAnyFloor()) {
        elevator_stopMotor();
        return EMERGENCY_STOP; 
    }
    if (p_firstOrder->floor < g_FLOOR && p_firstOrder->activeOrder) {
        elevator_goDown();
        return MOVE; 
    }
    if (p_firstOrder->floor > g_FLOOR && p_firstOrder->activeOrder) {
        elevator_goUp();
        return MOVE; 
    }
    if (p_firstOrder->floor == g_FLOOR && p_firstOrder->activeOrder) {
        switch(relativePosition) {
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


void s_move(){

    elevator_setRelativePosition(currentMoveDirection);
    elevator_checkAndAddOrder(g_FLOOR, currentMoveDirection);

    g_FLOOR = elevator_findCurrentFloor(g_FLOOR); // here floor inicator is set, bad decision?

    if (hardware_read_stop_signal() && elevator_amIAtAnyFloor()) {
        elevator_stopMotor();
        elevator_openDoors();
        return EMERGENCY_STOP; 
    }
    if (hardware_read_stop_signal() && !elevator_amIAtAnyFloor()) {
        elevator_stopMotor();
        return EMERGENCY_STOP; 
    }
    if (elevator_amIAtFloor(p_firstOrder->floor)) {
        elevator_stopMotor();
        return HANDLE_ORDER;
    }
}


void s_doorsOpenTimer(){ //OPEN DOORS OUTSIDE OF OPEN DOORS? (IN TRANSITION)

    time_t startTime = clock() * 1000/ CLOCKS_PER_SEC;

    while (startTime + DOORS_OPEN_TIME >= clock() * 1000/ CLOCKS_PER_SEC) {

        elevator_checkAndAddOrder(g_FLOOR,currentMoveDirection);

        if (hardware_read_stop_signal() && elevator_amIAtAnyFloor()) {
            elevator_stopMotor();
            elevator_openDoors();
            return EMERGENCY_STOP;
        }
        if (hardware_read_obstruction_signal()) { //unefficient to start timer every time?
            return DOORS_OPEN_TIMER;
        }
    }
    elevator_closeDoors();
    return IDLE;
}


void s_handleOrder() {     

    elevator_handleOrder();

    elevator_openDoors();
    return DOORS_OPEN_TIMER;
    //------------------------ PROBLEM: vil slukke lys og slette order etter timer, ikke før. -----------------------------------------------------
    //------------------------ Trenger handle order deale med emergency signal osv? --------------------------------------------------------------
}


void s_emergencyStop(){

    orderQueue_clear();
    hardware_command_stop_light(1);

    if (!hardware_read_stop_signal() && elevator_amIAtAnyFloor()) {
        elevator_openDoors();
        hardware_command_stop_light(0);
        return DOORS_OPEN_TIMER;
    }
    if (!hardware_read_stop_signal() && !elevator_amIAtAnyFloor()) {
        hardware_command_stop_light(0);
        return IDLE; 
    }
}



//-------------------------------------

int main(){

    int error = hardware_init();

    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    signal(SIGINT, sigint_handler);

    elevator_init();

    State state = IDLE;
    StateFunction p_state = s_idle;

    while(!elevator_amIAtAnyFloor()) {
        //Waiting 'till a floor is reached
    }

    while(1) {
        State nextState = p_state();
        switch(nextState){
            case IDLE:
                p_state = s_idle;
                break;
            case MOVING_DOWN:
                p_state = s_movingDown;
                break;
            case MOVING_UP:
                p_state = s_movingUp;
                break;
            case DOORS_OPEN:
                p_state = s_doorsOpen;
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