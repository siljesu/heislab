#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include "elevator.h"
#include "states.h"

static void sigint_handler(int sig){
    (void)(sig);
    printf("Terminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}

void s_idle(int floor, HardwareMovement moveDirection){

    int currentFloor = floor;
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    hardware_command_floor_indicator_on(currentFloor);
    HardwareMovement currentMoveDirection = moveDirection;

    while(1){
        if (hardware_read_stop_signal()) {
            s_emergencyStop(currentFloor,currentMoveDirection);
        }

        elevator_checkAndAddOrder(currentFloor, currentMoveDirection);

        if (!order_queue_empty()) {

            Order firstOrder = order_copy(order_queue[0]);

            if(firstOrder.floor < currentFloor){
                s_movingDown(currentFloor, HARDWARE_MOVEMENT_DOWN);
            } else if (firstOrder.floor > currentFloor){
                s_movingUp(currentFloor, HARDWARE_MOVEMENT_UP);
            } else if (firstOrder.floor == currentFloor){ //"enter elevator"
                s_handleOrder(currentFloor, currentMoveDirection);
            }
        }
    }
}

void s_movingDown(int floor, HardwareMovement moveDirection){

    int currentFloor = floor;
    hardware_command_movement(moveDirection);
    HardwareMovement currentMoveDirection = moveDirection;
    int targetFloor = order_queue[0].floor;
    
    while(1){
        if (hardware_read_stop_signal()) {
            s_emergencyStop(currentFloor,currentMoveDirection);
        }

        elevator_checkAndAddOrder(currentFloor, currentMoveDirection);
        
        currentFloor = elevator_findCurrentFloor(currentFloor);
        
        //new target?
        targetFloor = order_queue[0].floor;

        if (elevator_amIAtFloor(targetFloor)) {
            s_handleOrder(targetFloor, currentMoveDirection);
        }
    }
}


void s_movingUp(int floor, HardwareMovement moveDirection){

    int currentFloor = floor;
    hardware_command_movement(moveDirection);
    HardwareMovement currentMoveDirection = moveDirection;
    int targetFloor = order_queue[0].floor;
    
    while(1){
        if (hardware_read_stop_signal()) {
            s_emergencyStop(currentFloor,currentMoveDirection);
        }
        
        elevator_checkAndAddOrder(currentFloor, moveDirection);

        currentFloor = elevator_findCurrentFloor(currentFloor);

        //new target?
        targetFloor = order_queue[0].floor;

        if (elevator_amIAtFloor(targetFloor)) {
            s_handleOrder(targetFloor, currentMoveDirection);
        }
    }
}


void s_handleOrder(int floor, HardwareMovement moveDirection) {
    int currentFloor = floor;
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    hardware_command_floor_indicator_on(currentFloor);
    HardwareMovement lastMoveDirection = moveDirection;

    int threeSeconds = 3000;
    time_t startTime = clock() * 1000 / CLOCKS_PER_SEC;

    while (startTime + threeSeconds >= clock()* 1000 / CLOCKS_PER_SEC) {
        hardware_command_door_open(1);
        
        //need to check for things
        elevator_checkAndAddOrder(currentFloor, lastMoveDirection);
        if (hardware_read_stop_signal()) {
            s_emergencyStop(currentFloor, lastMoveDirection);
        }
        if (hardware_read_obstruction_signal()) {
            s_obstruction(currentFloor, lastMoveDirection);
            break;
        }
    }

    hardware_command_door_open(0);
    order_queue_deleteByShifting();
    for (int i = 0; i < 2; i++) {
        if (order_queue[0].floor == currentFloor) {
            order_queue_deleteByShifting();
        }
    }

    s_idle(currentFloor, lastMoveDirection);

}


void s_emergencyStop(int floor, HardwareMovement moveDirection){

    int currentFloor = floor;
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    HardwareMovement lastMoveDirection = moveDirection;
    
    order_queue_clear();

    while(hardware_read_stop_signal()){
        
        hardware_command_stop_light(1);
        
        if (elevator_amIAtFloor(currentFloor)) {
            //then you are on a floor, and must open doors.
            hardware_command_door_open(1);
            
        } else {
            //then you are in the middle of floors, with currentFloor as the last floor you were at, going in movementDirection. Must close doors.
            hardware_command_door_open(0);
        }
    }

    hardware_command_stop_light(0);
    
    //exit actions
    if (elevator_amIAtFloor(currentFloor)) {
        //create 3 sec timer. remember to check for obstruction.
        int threeSeconds = 3000;
        time_t startTime = clock() * 1000/ CLOCKS_PER_SEC;

        while (startTime + threeSeconds >= clock() * 1000/ CLOCKS_PER_SEC) {
            hardware_command_door_open(1);
            
            //need to check for things
            elevator_checkAndAddOrder(currentFloor,lastMoveDirection);
            if (hardware_read_stop_signal()) {
                s_emergencyStop(currentFloor,lastMoveDirection);
            }
            if (hardware_read_obstruction_signal()) {
                s_obstruction(currentFloor,lastMoveDirection);
            }
        }

        hardware_command_door_open(0);
        s_idle(currentFloor,lastMoveDirection);

    } else {
        //ready, in the middle of floors; dont need to worry about doors.
        s_idleInBetweenFloors(currentFloor,lastMoveDirection);

    }
}

void s_obstruction(int floor, HardwareMovement moveDirection){

    int currentFloor = floor;
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    HardwareMovement lastMoveDirection = moveDirection;
    
    while(hardware_read_obstruction_signal()){
        hardware_command_door_open(1);
        elevator_checkAndAddOrder(currentFloor,lastMoveDirection);
        if (hardware_read_stop_signal()) {
            s_emergencyStop(currentFloor,lastMoveDirection);
        }

    }
    
    //exit actions
    //create 3 sec timer. remember to check for obstruction.
    int threeSeconds = 3000;
    time_t startTime = clock() * 1000/ CLOCKS_PER_SEC;

    while (startTime + threeSeconds >= clock() * 1000/ CLOCKS_PER_SEC) {
        hardware_command_door_open(1);

        //need to check for things
        elevator_checkAndAddOrder(currentFloor,lastMoveDirection);

        if (hardware_read_stop_signal()) {
            s_emergencyStop(currentFloor,lastMoveDirection);
        }
        if (hardware_read_obstruction_signal()) {
            s_obstruction(currentFloor,lastMoveDirection);
        }
    }

    hardware_command_door_open(0);
    //s_idle(currentFloor,lastMoveDirection);
}


void s_idleInBetweenFloors(int floor, HardwareMovement moveDirection){
    int lastFloor = floor;
    HardwareMovement lastMoveDirection = moveDirection;

    while(1) {
        elevator_checkAndAddOrder(lastFloor, lastMoveDirection);

        if (hardware_read_stop_signal()) {
            s_emergencyStop(lastFloor, lastMoveDirection);
        }

        if (!order_queue_empty()) {

            Order firstOrder = {order_queue[0].floor, order_queue[0].order_type, order_queue[0].emptyOrder};

            if(firstOrder.floor < lastFloor){
                s_movingDown(lastFloor, HARDWARE_MOVEMENT_DOWN);
            } else if (firstOrder.floor > lastFloor){
                s_movingUp(lastFloor, HARDWARE_MOVEMENT_UP);
            } else {
                //order in *your* floor, are you above or below?
                if (lastMoveDirection == HARDWARE_MOVEMENT_UP) {
                    s_movingDown(lastFloor, HARDWARE_MOVEMENT_DOWN);
                }
                else if (lastMoveDirection == HARDWARE_MOVEMENT_DOWN) {
                    s_movingUp(lastFloor, HARDWARE_MOVEMENT_UP);
                }
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

    while(1){
        for (int i = 0; i < NUMBER_OF_FLOORS; i++){
            if (hardware_read_floor_sensor(i)){
                s_idle(i,initialMovement);
            }
        }        
    }

    return 0;
}