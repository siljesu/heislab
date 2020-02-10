#include <stdio.h>
#include <stdlib.h>
#include "hardware.h"
#include "order.h"
#include "order_queue.h"
#include "elevator.h"



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
            s_idle(i,initialMovement,/*order?*/);
        }
    }
}







//state, bare uten rare-case obstruksjon
void s_idle(int floor, HardwareMovement moveDirection, struct Order firstOrder){

    int currentFloor = floor;
    HardwareMovement currentMoveDirection = moveDirection;

    while(1){
        if (hardware_read_stop_signal()) {
            s_emergencyStop(currentFloor,currentMoveDirection);
        }

        elevator_checkAndAddOrder(currentFloor, moveDirection);

        if (!order_queue_empty) {

            struct Order firstOrder = order_queue[0];

            if(firstOrder.floor < currentFloor){
                s_movingDown(currentFloor, HARDWARE_MOVEMENT_DOWN,firstOrder); //hardkodet movement?
            } else if (firstOrder.floor > currentFloor){
                s_movingUp(currentFloor, HARDWARE_MOVEMENT_UP,firstOrder); //hardkodet movement?
            }
        }
    }
}

void s_movingDown(int floor, HardwareMovement moveDirection, struct Order firstOrder){

    int currentFloor = floor;
    hardware_command_movement(moveDirection);
    HardwareMovement currentMoveDirection = moveDirection;
    int targetFloor = firstOrder.floor;
    
    while(1){
        if (hardware_read_stop_signal()) {
            s_emergencyStop();
        }

        elevator_checkAndAddOrder(currentFloor, moveDirection);
        
        //new target?
        order_queue[0].floor = targetFloor;

        if (elevator_amIAtFloor(targetFloor)) {
            s_handleOrder(targetFloor, HARDWARE_MOVEMENT_STOP, firstOrder);
        }
    }
}


void s_movingUp(int floor, HardwareMovement moveDirection, struct Order firstOrder){

    int currentFloor = floor;
    hardware_command_movement(moveDirection);
    HardwareMovement currentMoveDirection = moveDirection;
    int targetFloor = firstOrder.floor;
    
    while(1){
        if (hardware_read_stop_signal()) {
            s_emergencyStop();
        }
        
        elevator_checkAndAddOrder(currentFloor, moveDirection);

        //new target?
        order_queue[0].floor = targetFloor;

        if (elevator_reachedDestinationYet(targetFloor)) {
            s_handleOrder(targetFloor, HARDWARE_MOVEMENT_STOP, firstOrder);
        }
    }
}


void s_emergencyStop(int floor, HardwareMovement moveDirection, struct Order firstOrder){

    int currentFloor = floor;
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    
    while(1){
        
        if (elevator_amIAtFloor(currentFloor)) {
            //then you are on a floor, and must open doors.

        } else {
            //then you are in the middle of floors, with currentFloor as the last floor you were at, going in movementDirection. Must close doors.

        }

        if (hardware_read_stop_signal()) {
            s_emergencyStop();

        } else {
            //stop signal released. 

            if (elevator_amIAtFloor(currentFloor)) {
                //create 3 sec timer. remember to check for obstruction.
                s_idle();

            } else {
                //ready, in the middle of floors; dont need to worry about doors.
                s_idleInBetweenFloors();

            }

        }
    }
}

s_idleInBetweenFloors(int floor, HardwareMovement moveDirection, struct Order firstOrder){
    int lastFloor = floor;
    HardwareMovement lastMoveDirection = moveDirection;
    //whereAmI?
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
    struct Order* order;
    for (int i = 0; i < NUMBER_OF_FLOORS; i++){
        if (hardware_read_order(i, HARDWARE_ORDER_UP)){
            order.order_init(i,HARDWARE_ORDER_UP);
            order_queue_add_order(order,currentFloor,moveDirection);
        }
        if (hardware_read_order(i, HARDWARE_ORDER_DOWN)){
            order.order_init(i,HARDWARE_ORDER_DOWN);
            order_queue_add_order(order,currentFloor,moveDirection);
        }
        if (hardware_read_order(i, HARDWARE_ORDER_INSIDE)){
            order.order_init(i,HARDWARE_ORDER_INSIDE);
            order_queue_add_order(order,currentFloor,moveDirection);
        }
    }
}