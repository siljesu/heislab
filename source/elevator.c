#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "order.h"
#include "order_queue.h"
#include "elevator.h"
#include <time.h>



int main(){
    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    //start at valid state?
    HardwareMovement initialMovement = HARDWARE_MOVEMENT_DOWN;
    hardware_command_movement(initialMovement);

    for (int i = 0; i < NUMBER_OF_FLOORS; i++){
        if (hardware_read_floor_sensor(i)){
            s_idle(i,initialMovement);
        }
    }
}



void s_idle(int floor, HardwareMovement moveDirection){

    int currentFloor = floor;
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    HardwareMovement currentMoveDirection = moveDirection;

    while(1){
        if (hardware_read_stop_signal()) {
            s_emergencyStop(currentFloor,currentMoveDirection);
        }

        elevator_checkAndAddOrder(currentFloor, currentMoveDirection);

        if (!order_queue_empty) {

            struct Order firstOrder = {order_queue[0].floor, order_queue[0].order_type, order_queue[0].emptyOrder};

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

        //new target?
        order_queue[0].floor = targetFloor;

        if (elevator_amIAtFloor(targetFloor)) {
            s_handleOrder(targetFloor, currentMoveDirection);
        }
    }
}


void s_handleOrder(int floor, HardwareMovement moveDirection) {
    int currentFloor = floor;
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    HardwareMovement lastMoveDirection = moveDirection;

    int threeSeconds = 3000;
    time_t startTime = clock();

    while (startTime + threeSeconds >= clock()) {
        hardware_command_door_open(1);
        
        //need to check for things
        elevator_checkAndAddOrder(currentFloor, lastMoveDirection);
        if (hardware_read_stop_signal()) {
            s_emergencyStop(currentFloor, lastMoveDirection);
        }
        if (hardware_read_obstruction_signal()) {
            s_obstruction(currentFloor, lastMoveDirection);
        }
    }
    order_queue_shift();
    hardware_command_door_open(0);
    s_idle(currentFloor, lastMoveDirection);

}


void s_emergencyStop(int floor, HardwareMovement moveDirection){

    int currentFloor = floor;
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    HardwareMovement lastMoveDirection = moveDirection;
    
    order_queue_clear();

    while(hardware_read_stop_signal()){
        
        if (elevator_amIAtFloor(currentFloor)) {
            //then you are on a floor, and must open doors.
            hardware_command_door_open(1);
            
        } else {
            //then you are in the middle of floors, with currentFloor as the last floor you were at, going in movementDirection. Must close doors.
            hardware_command_door_open(0);
        }
    }
    
    //exit actions
    if (elevator_amIAtFloor(currentFloor)) {
        //create 3 sec timer. remember to check for obstruction.
        int threeSeconds = 3000;
        time_t startTime = clock();

        while (startTime + threeSeconds >= clock()) {
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
    }
    
    //exit actions
    //create 3 sec timer. remember to check for obstruction.
    int threeSeconds = 3000;
    time_t startTime = clock();

    while (startTime + threeSeconds >= clock()) {
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
}


s_idleInBetweenFloors(int floor, HardwareMovement moveDirection){
    int lastFloor = floor;
    HardwareMovement lastMoveDirection = moveDirection;

    while(1) {
        elevator_checkAndAddOrder(lastFloor, lastMoveDirection);

        if (hardware_read_stop_signal()) {
            s_emergencyStop(lastFloor, lastMoveDirection);
        }

        if (!order_queue_empty) {

            struct Order firstOrder = {order_queue[0].floor, order_queue[0].order_type, order_queue[0].emptyOrder};

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





int elevator_amIAtFloor(targetFloor){
    for (int i = 0; i < NUMBER_OF_FLOORS; i++){
        if (hardware_read_floor_sensor(i) && i == targetFloor) {
            return 1;
        }
    }
    return 0;
}


void elevator_checkAndAddOrder(int currentFloor, HardwareMovement moveDirection){
    struct Order *order;
    for (int i = 0; i < NUMBER_OF_FLOORS; i++){
        if (hardware_read_order(i, HARDWARE_ORDER_UP)){
            order_init(*order,i,HARDWARE_ORDER_UP);
            order_queue_add_order(*order,currentFloor,moveDirection);
        }
        if (hardware_read_order(i, HARDWARE_ORDER_DOWN)){
            order_init(*order,i,HARDWARE_ORDER_DOWN);
            order_queue_add_order(*order,currentFloor,moveDirection);
        }
        if (hardware_read_order(i, HARDWARE_ORDER_INSIDE)){
            order_init(*order,i,HARDWARE_ORDER_INSIDE);
            order_queue_add_order(*order,currentFloor,moveDirection);
        }
    }
}

