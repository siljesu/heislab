#include <stdio.h>
#include <stdlib.h>
#include "elevator.h"


extern RelativePosition* p_relative_position = &relative_position;


void elevator_init() {
    hardware_command_clear_all_order_lights();

    int local_queue_size = 12;
    for (int i = 0; i < (local_queue_size); i++){
        orderQueue[i] = EMPTYORDER;
       	goingUp[i] = EMPTYORDER;
		goingDown[i] = EMPTYORDER;
		secondGoingUp[i] = EMPTYORDER;
		secondGoingDown[i] = EMPTYORDER;
    }

    for (int i = 0; i < NUMBER_OF_FLOORS; i++){
        for (int j = 0; j < NUMBER_OF_ORDER_TYPES; j++){
            orderTable[i][j] = 0;
        }
    }
}

int elevator_findCurrentFloor(int lastFloor){
    for (int f = 0; f < NUMBER_OF_FLOORS; f++){
        if (hardware_read_floor_sensor(f)){
            hardware_command_floor_indicator_on(f);
            return f;
        }
    }
    return lastFloor;
}

int elevator_amIAtFloor(int targetFloor){
    for (int i = 0; i < NUMBER_OF_FLOORS; i++){
        if (hardware_read_floor_sensor(i) && (i == targetFloor)) {
            return 1;
        }
    }
    return 0;
}

int elevator_amIAtAnyFloor(){
    for (int i = 0; i < NUMBER_OF_FLOORS; i++){
       if(hardware_read_floor_sensor(i)){
            return 1;      
        }
    }
    return 0;
}


void elevator_checkAndAddOrder(int currentFloor, HardwareMovement moveDirection){
    for (int i = 0; i < NUMBER_OF_FLOORS; i++){
        if (hardware_read_order(i, HARDWARE_ORDER_UP)){
            Order order = {order_init(i,HARDWARE_ORDER_UP).floor, order_init(i,HARDWARE_ORDER_UP).order_type, order_init(i,HARDWARE_ORDER_UP).activeOrder};
            if (order_checkUnique(order)) {
                Order *p_order = &order;
                orderQueue_addOrder(p_order,currentFloor,moveDirection);
                order_toggleUnique(order,1);
            }
        }
        if (hardware_read_order(i, HARDWARE_ORDER_DOWN)){
            Order order = {order_init(i,HARDWARE_ORDER_DOWN).floor, order_init(i,HARDWARE_ORDER_DOWN).order_type, order_init(i,HARDWARE_ORDER_DOWN).activeOrder};
            if (order_checkUnique(order)) {
                Order *p_order = &order;
                orderQueue_addOrder(p_order,currentFloor,moveDirection);
                order_toggleUnique(order,1);
            }
        }
        if (hardware_read_order(i, HARDWARE_ORDER_INSIDE)){
            Order order = {order_init(i,HARDWARE_ORDER_INSIDE).floor, order_init(i,HARDWARE_ORDER_INSIDE).order_type, order_init(i,HARDWARE_ORDER_INSIDE).activeOrder};
            if (order_checkUnique(order)) {
                Order *p_order = &order;
                orderQueue_addOrder(p_order,currentFloor,moveDirection);
                order_toggleUnique(order,1);
            }
        }
    }
}

void elevator_setRelativePosition(HardwareMovement moveDirection){
    if (moveDirection == HARDWARE_MOVEMENT_UP) {
        if (relative_position == AT && (!elevator_amIAtAnyFloor())){
            *p_relative_position = ABOVE;
        }
        else if (relative_position != AT && (elevator_amIAtAnyFloor())){
            *p_relative_position = AT;
        } 
    } 
    else if (moveDirection == HARDWARE_MOVEMENT_DOWN) {
        if (relative_position == AT && (!elevator_amIAtAnyFloor())){
            *p_relative_position = BELOW;
        }
        else if (relative_position != AT && (elevator_amIAtAnyFloor())){
            *p_relative_position = AT;
        } 
    }
}