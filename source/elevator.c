#include <stdio.h>
#include <stdlib.h>
#include "elevator.h"

void elevator_init() {
    hardware_command_clear_all_order_lights();

    int local_queue_size = 12;
    for (int i = 0; i < (local_queue_size); i++){
        order_queue[i] = EMPTYORDER;
       	going_up[i] = EMPTYORDER;
		going_down[i] = EMPTYORDER;
		second_going_up[i] = EMPTYORDER;
		second_going_down[i] = EMPTYORDER;
    }

    for (int i = 0; i < NUMBER_OF_FLOORS; i++){
        for (int j = 0; j < NUMBER_OF_ORDER_TYPES; j++){
            order_table[i][j] = 0;
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


void elevator_checkAndAddOrder(int currentFloor, HardwareMovement moveDirection){
    for (int i = 0; i < NUMBER_OF_FLOORS; i++){
        if (hardware_read_order(i, HARDWARE_ORDER_UP)){
            Order order = {order_init(i,HARDWARE_ORDER_UP).floor, order_init(i,HARDWARE_ORDER_UP).order_type, order_init(i,HARDWARE_ORDER_UP).emptyOrder};
            if (order_check_unique(order)) {
                Order *p_order = &order;
                order_queue_add_order(p_order,currentFloor,moveDirection);
                order_toggle_unique(order,1);
            }
        }
        if (hardware_read_order(i, HARDWARE_ORDER_DOWN)){
            Order order = {order_init(i,HARDWARE_ORDER_DOWN).floor, order_init(i,HARDWARE_ORDER_DOWN).order_type, order_init(i,HARDWARE_ORDER_DOWN).emptyOrder};
            if (order_check_unique(order)) {
                Order *p_order = &order;
                order_queue_add_order(p_order,currentFloor,moveDirection);
                order_toggle_unique(order,1);
            }
        }
        if (hardware_read_order(i, HARDWARE_ORDER_INSIDE)){
            Order order = {order_init(i,HARDWARE_ORDER_INSIDE).floor, order_init(i,HARDWARE_ORDER_INSIDE).order_type, order_init(i,HARDWARE_ORDER_INSIDE).emptyOrder};
            if (order_check_unique(order)) {
                Order *p_order = &order;
                order_queue_add_order(p_order,currentFloor,moveDirection);
                order_toggle_unique(order,1);
            }
        }
    }
}

