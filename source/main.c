#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include "elevator.h"

typedef enum {BELOW, ABOVE, AT} RelativePosition;
static RelativePosition relative_position;
//static RelativePosition* p_relative_position = &relative_position;

state p_state;
/*typedef enum {IDLE, MOVING_DOWN, MOVING_UP, HANDLE_ORDER, EMERGENCY_STOP, IDLE_BETWEEN_FLOORS, OBSTRUCTION} State;
static State currentState;
State* p_currentState = &currentState;*/

static int FLOOR;
    int FLOOR;
    //int* p_floor = &floor;
    static HardwareMovement currentMoveDirection;
    HardwareMovement currentMoveDirection;
    //HardwareMovement* p_currentMoveDirection = &currentMoveDirection;

static void sigint_handler(int sig){
    (void)(sig);
    printf("Terminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}

void s_idle(){

    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    hardware_command_floor_indicator_on(FLOOR);
    relative_position = AT;

    while(1){
        if (hardware_read_stop_signal()) {
            p_state = &s_emergencyStop;
            return; //We don't need to break and go out of scope - return does this for us
        }

        elevator_checkAndAddOrder(FLOOR, currentMoveDirection);

        if (!orderQueue_empty()) {

            if(p_firstOrder->floor < FLOOR){
                p_state = &s_movingDown;
                return;
            } else if (p_firstOrder->floor > FLOOR){
                p_state = &s_movingUp;
                return;
            } else if (p_firstOrder->floor == FLOOR){ //"enter elevator"
                p_state = &s_handleOrder;
                return;
            }
        }
    }
}

void s_movingDown(){

    hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    currentMoveDirection = HARDWARE_MOVEMENT_DOWN;
    
    while(1){
        if (hardware_read_stop_signal()) {
            p_state = &s_emergencyStop;
            return;
        }

        elevator_checkAndAddOrder(FLOOR, currentMoveDirection);
        
        FLOOR = elevator_findCurrentFloor(FLOOR);

        if (relative_position == AT && (!elevator_amIAtAnyFloor())){
            relative_position = BELOW;
        }
        else if (relative_position != AT && (elevator_amIAtAnyFloor())){
            relative_position = AT;
        } 
        
        //new target?
        int targetFloor = p_firstOrder->floor;

        if (elevator_amIAtFloor(targetFloor)) {
            p_state = &s_handleOrder;
            return;
        }
    }
}


void s_movingUp(int FLOOR, HardwareMovement currentMoveDirection){

    hardware_command_movement(HARDWARE_MOVEMENT_UP);
    currentMoveDirection = HARDWARE_MOVEMENT_UP;
    
    while(1){
        if (hardware_read_stop_signal()) {
            p_state = &s_emergencyStop;
            return;
        }
        
        elevator_checkAndAddOrder(FLOOR, currentMoveDirection);

        FLOOR = elevator_findCurrentFloor(FLOOR);

        if (relative_position == AT && (!elevator_amIAtAnyFloor())){
            relative_position = ABOVE;
        }
        else if (relative_position != AT && (elevator_amIAtAnyFloor())){
            relative_position = AT;
        } 

        //new target?
        int targetFloor = p_firstOrder->floor;

        if (elevator_amIAtFloor(targetFloor)) {
            p_state = &s_handleOrder;
            return;
        }
    }
}


void s_handleOrder() {     

    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    hardware_command_floor_indicator_on(FLOOR);

    int threeSeconds = 3000;
    time_t startTime = clock() * 1000 / CLOCKS_PER_SEC;

    while (startTime + threeSeconds >= clock()* 1000 / CLOCKS_PER_SEC) {
        hardware_command_door_open(1);
        
        //need to check for things
        elevator_checkAndAddOrder(FLOOR, currentMoveDirection);
        if (hardware_read_stop_signal()) {
            p_state = &s_emergencyStop;
            return;
        }
        if (hardware_read_obstruction_signal()) {
            p_state = &s_obstruction;
            return;
        }
    }

    hardware_command_door_open(0);
    orderQueue_deleteByShiftingAtIndex(0);
    for (int i = 0; i < 12; i++) { //hardcoded queuesize
        if (orderQueue[i].floor == FLOOR) {
            orderQueue_deleteByShiftingAtIndex(i);
        }
    }

    p_state = &s_idle;

}


void s_emergencyStop(){

    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    
    orderQueue_clear();

    while(hardware_read_stop_signal()){
        
        hardware_command_stop_light(1);
        
        if (elevator_amIAtFloor(FLOOR)) {
            //then you are on a floor, and must open doors.
            hardware_command_door_open(1);
            
        } else {
            //then you are in the middle of floors, with FLOOR as the last floor you were at, going in movementDirection. Must close doors.
            hardware_command_door_open(0);
        }
    }

    hardware_command_stop_light(0);
    
    //exit actions
    if (elevator_amIAtFloor(FLOOR)) {
        //create 3 sec timer. remember to check for obstruction.
        int threeSeconds = 3000;
        time_t startTime = clock() * 1000/ CLOCKS_PER_SEC;

        while (startTime + threeSeconds >= clock() * 1000/ CLOCKS_PER_SEC) {
            hardware_command_door_open(1);
            
            //need to check for things
            elevator_checkAndAddOrder(FLOOR, currentMoveDirection);
            if (hardware_read_stop_signal()) {
                p_state = &s_emergencyStop;
                return;
            }
            if (hardware_read_obstruction_signal()) {
                p_state = &s_obstruction;
                return;
            }
        }

        hardware_command_door_open(0);
        p_state = &s_idle;
        return;

    } else {
        //ready, in the middle of floors; dont need to worry about doors.
        p_state = &s_idleInBetweenFloors;
        return;
    }
}

void s_obstruction(){

    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    
    while(hardware_read_obstruction_signal()){
        hardware_command_door_open(1);
        elevator_checkAndAddOrder(FLOOR,currentMoveDirection);
        if (hardware_read_stop_signal()) {
            p_state = &s_emergencyStop;
            return;
        }

    }
    
    //exit actions
    //create 3 sec timer. remember to check for obstruction.
    int threeSeconds = 3000;
    time_t startTime = clock() * 1000/ CLOCKS_PER_SEC;

    while (startTime + threeSeconds >= clock() * 1000/ CLOCKS_PER_SEC) {
        hardware_command_door_open(1);

        //need to check for things
        elevator_checkAndAddOrder(FLOOR, currentMoveDirection);

        if (hardware_read_stop_signal()) {
            p_state = &s_emergencyStop;
            return;
        }
        if (hardware_read_obstruction_signal()) {
            p_state = &s_obstruction;
            return;
        }
    }

    hardware_command_door_open(0);
    p_state = &s_idle;
    return;
}


void s_idleInBetweenFloors(){

    while(1) {
        elevator_checkAndAddOrder(FLOOR, currentMoveDirection);

        if (hardware_read_stop_signal()) {
            p_state = &s_emergencyStop;
            return;
        }

        if (!orderQueue_empty()) {

            Order firstOrder = {p_firstOrder->floor, p_firstOrder->order_type, p_firstOrder->activeOrder};

            

            if(firstOrder.floor < FLOOR){
                p_state = &s_movingDown;
                return;
            } else if (firstOrder.floor > FLOOR){
                p_state = &s_movingUp;
                return;
            } else {
                switch(relative_position){
                case BELOW:
                    p_state = &s_movingUp;
                    break;
                case ABOVE:
                    p_state = &s_movingDown;
                    break;
                case AT:
                    p_state = &s_idle; //Filler values; states must be rewritten anyways
                    break;
                }
                return;
            }
        }
    }
}

int main(){
    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    signal(SIGINT, sigint_handler);

    elevator_init();

    //start at valid state?
    HardwareMovement initialMovement = HARDWARE_MOVEMENT_DOWN;
    hardware_command_movement(initialMovement);
    for (int i = 0; i < NUMBER_OF_FLOORS; i++){
        if (hardware_read_floor_sensor(i)){
            FLOOR = i;
            p_state = &s_idle;
            break;
        }
    } 

    while(1){
        p_state();       
    }

    return 0;
}
/*
int main(){
    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    signal(SIGINT, sigint_handler);

    elevator_init();

    //start at valid state?
    HardwareMovement initialMovement = HARDWARE_MOVEMENT_DOWN;
    hardware_command_movement(initialMovement);

    while(1){
        for (int i = 0; i < NUMBER_OF_FLOORS; i++){
            if (hardware_read_floor_sensor(i)){
                s_idle(i,initialMovement);
            }
        }        
    }

    return 0;
}*/