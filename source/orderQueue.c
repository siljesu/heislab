/**
 * @file
 * @brief orderQueue module's implementation
 *
**/

#include <stdio.h>
#include "orderQueue.h"

#ifndef QUEUE_SIZE
#define QUEUE_SIZE 12
#endif

RelativePosition g_relativePosition;

void orderQueue_clear(){
	for (int i = 0; i < QUEUE_SIZE ; i++){
		orderQueue[i] = order_delete(orderQueue[i]);
	}
}

void orderQueue_deleteByShiftingAtIndex(int i){
	orderQueue[i] = order_delete(orderQueue[i]);
	for (int j = i; j < (QUEUE_SIZE - 1); j++){
		orderQueue[j] = order_copy(orderQueue[j + 1]);
	}
	orderQueue[QUEUE_SIZE - 1] = EMPTYORDER;
}

void orderQueue_selectionSort(Order* temp_array, bool incrementing){
	Order temp_order;
	int index;

	if (incrementing){ 
		for (int i = 0; i < QUEUE_SIZE; i++){
			index = i;
			temp_order = temp_array[i];
			for (int j = i; j < QUEUE_SIZE; j++){
				if ( (temp_array[j].activeOrder) && (temp_array[j].floor < temp_order.floor) ){
					temp_order = temp_array[j];
					index = j;
				}
			}
			temp_array[index] = temp_array[i];
			temp_array[i] = temp_order;
		}
	}

	else if (!incrementing){
		for (int i = 0; i < QUEUE_SIZE; i++){
			index = i;
			temp_order = temp_array[i];
			for (int j = i; j < QUEUE_SIZE; j++){
				if ( (temp_array[j].activeOrder) && (temp_array[j].floor > temp_order.floor) ){
					temp_order = temp_array[j];
					index = j;
				}
			}
			temp_array[index] = temp_array[i];
			temp_array[i] = temp_order;
		}
	}

}

void orderQueue_resetSortingArrays(){
	for (int i = 0; i < QUEUE_SIZE; i++){
		goingUp[i] = EMPTYORDER;
		goingDown[i] = EMPTYORDER;
		secondGoingUp[i] = EMPTYORDER;
		secondGoingDown[i] = EMPTYORDER;
	}
}

void orderQueue_sortChunksByDirection(int countUp,int countDown,int countSecondUp,int countSecondDown,HardwareMovement direction){

	orderQueue_selectionSort(goingUp, true);
	orderQueue_selectionSort(goingDown, false);
	orderQueue_selectionSort(secondGoingUp, true);
	orderQueue_selectionSort(secondGoingDown, false);

	int offset = 0;
    int end_border;

	switch(direction) {
		case HARDWARE_MOVEMENT_UP:

			end_border = countUp;

			for (int i = offset; i < end_border; i++){
				orderQueue[i] = goingUp[i - offset];
			}

			offset += countUp;
			end_border += countDown;
			for (int i = offset; i < end_border; i++){
				orderQueue[i] = goingDown[i - offset];
			}
			
			offset += countDown;
			end_border += countSecondUp;
			for (int i = offset; i < end_border; i++){
				orderQueue[i] = secondGoingUp[i - offset];
			}
			
			offset += countSecondUp; 
			end_border += countSecondDown;
			for (int i = offset; i < end_border; i++){
				orderQueue[i] = secondGoingDown[i - offset];
			}
			break;
		
		case HARDWARE_MOVEMENT_DOWN:

			end_border = countDown;

			for (int i = offset; i < end_border; i++){
				orderQueue[i] = goingDown[i - offset];
			}

			offset += countDown;
			end_border += countUp;
			for (int i = offset; i < end_border; i++){
				orderQueue[i] = goingUp[i - offset];
			}
			
			offset += countUp;
			end_border += countSecondDown;
			for (int i = offset; i < end_border; i++){
				orderQueue[i] = secondGoingDown[i - offset];
			}
			
			offset += countSecondDown; 
			end_border += countSecondUp;
			for (int i = offset; i < end_border; i++){
				orderQueue[i] = secondGoingUp[i - offset];
			}
			break;

		case HARDWARE_MOVEMENT_STOP:

			printf("ERROR: logic error, movement stop is not a direction: going into idle.");

			break;
	}

	orderQueue_resetSortingArrays();
}

void orderQueue_sortOrderQueue(int elevator_floor, HardwareMovement direction){

	int countUp = 0;
	int countDown = 0;
	int countSecondUp = 0;
	int countSecondDown = 0;

	for (int i = 0; i < (QUEUE_SIZE); i++){
		if ((orderQueue + i)->activeOrder){
			int order_floor = orderQueue[i].floor;
			HardwareOrder order_type = orderQueue[i].order_type;
			int atFloor = hardware_read_floor_sensor(elevator_floor);

			switch (direction) {
				case HARDWARE_MOVEMENT_UP:

					//Include orders at current floor
					if (atFloor){ 
						if ( (order_floor >= elevator_floor) && (order_type == HARDWARE_ORDER_UP || order_type == HARDWARE_ORDER_INSIDE) ){
							goingUp[countUp] = orderQueue[i];
							countUp++;
						}
						else if ( (order_type == HARDWARE_ORDER_DOWN) || ((order_floor < elevator_floor) && order_type == HARDWARE_ORDER_INSIDE) ){
							goingDown[countDown] = orderQueue[i];
							countDown++;
						}
						else if ( (order_floor < elevator_floor) && (order_type == HARDWARE_ORDER_UP) ){
							secondGoingUp[countSecondUp] = orderQueue[i];
							countSecondUp++;
						}
					//Don't include orders at current floor
					} else { 
						
						if ( (order_floor > elevator_floor) && (order_type == HARDWARE_ORDER_UP || order_type == HARDWARE_ORDER_INSIDE) ){
							goingUp[countUp] = orderQueue[i];
							countUp++;
						}
						else if ( (order_type == HARDWARE_ORDER_DOWN) || ((order_floor <= elevator_floor) && order_type == HARDWARE_ORDER_INSIDE) ){
							goingDown[countDown] = orderQueue[i];
							countDown++;
						}
						else if ( (order_floor <= elevator_floor) && (order_type == HARDWARE_ORDER_UP) ){
							//As a safeguard, we also need to check relative position
							if ((g_relativePosition == BELOW) && (order_floor == elevator_floor)){
								goingUp[countUp] = orderQueue[i];
								countUp++;
							}
							else{
								secondGoingUp[countSecondUp] = orderQueue[i];
								countSecondUp++;
							}
						}
					}

					break;
				case HARDWARE_MOVEMENT_DOWN:

					//Include orders at current floor
					if (atFloor){
						if ( (order_floor <= elevator_floor) && (order_type == HARDWARE_ORDER_DOWN || order_type == HARDWARE_ORDER_INSIDE) ){
							goingDown[countDown] = orderQueue[i];
							countDown++;
						}
						else if ( (order_type == HARDWARE_ORDER_UP) || ((order_floor > elevator_floor) && order_type == HARDWARE_ORDER_INSIDE) ){
							goingUp[countUp] = orderQueue[i];
							countUp++;
						}
						else if ( (order_floor > elevator_floor) && (order_type == HARDWARE_ORDER_DOWN) ){
							secondGoingDown[countSecondDown] = orderQueue[i];
							countSecondDown++;
						}
					//Don't include orders at current floor
					} else {
						if ( (order_floor < elevator_floor) && (order_type == HARDWARE_ORDER_DOWN || order_type == HARDWARE_ORDER_INSIDE) ){
							goingDown[countDown] = orderQueue[i];
							countDown++;
						}
						else if ( (order_type == HARDWARE_ORDER_UP) || ((order_floor >= elevator_floor) && order_type == HARDWARE_ORDER_INSIDE) ){
							goingUp[countUp] = orderQueue[i];
							countUp++;
						}
						else if ( (order_floor >= elevator_floor) && (order_type == HARDWARE_ORDER_DOWN) ){
							//As a safeguard, we also need to check relative position
							if ((g_relativePosition == ABOVE) && (order_floor == elevator_floor)){
								goingDown[countDown] = orderQueue[i];
								countDown++;
							}
							else{
								secondGoingDown[countSecondDown] = orderQueue[i];
								countSecondDown++;
							}	
						}
					}

					break;
				case HARDWARE_MOVEMENT_STOP:

					printf("ERROR: logic error, movement stop is not a direction: going into idle.");

					break;
			}
		}
	}
	orderQueue_sortChunksByDirection(countUp, countDown, countSecondUp, countSecondDown, direction);
}

void orderQueue_addOrder(Order order, int elevator_floor, HardwareMovement direction){
	orderQueue[QUEUE_SIZE-1] = order_copy(order);
	orderQueue_sortOrderQueue(elevator_floor, direction);
	orderQueue[QUEUE_SIZE-1] = order_delete(orderQueue[QUEUE_SIZE-1]);
}