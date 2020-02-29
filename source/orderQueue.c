#include <stdio.h>
#include "orderQueue.h"

#define QUEUE_SIZE 12

Order* p_firstOrder = &(orderQueue[0]);

void orderQueue_clear(){
	for (int i = 0; i < QUEUE_SIZE ; i++){
		orderQueue[i] = order_delete(orderQueue[i]);
	}
}

int orderQueue_empty(){
	for (int i = 0; i < (QUEUE_SIZE - 1); i++){
		if (orderQueue[i].activeOrder)
			return 0;
	}
	return 1;
}

void orderQueue_deleteByShiftingAtIndex(int i){
	orderQueue[i] = order_delete(orderQueue[i]);
	for (int j = i; j < (QUEUE_SIZE - 1); j++){
		orderQueue[j] = order_copy(orderQueue[j + 1]);
	}
	orderQueue[QUEUE_SIZE - 1] = EMPTYORDER;
}

void orderQueue_sortIncrementally(Order* temp_array, bool increasing){
	Order temp_order;
	int index;

	if (increasing){
		for (int i = 0; i < QUEUE_SIZE; i++){
			index = i;
			temp_order = *(temp_array + i);
			for (int j = i; j < QUEUE_SIZE; j++){
				if ( ((temp_array + j)->activeOrder) && ((temp_array + j)->floor < temp_order.floor) ){
					temp_order = *(temp_array + j);
					index = j;
				}
			}
			*(temp_array + index) = *(temp_array + i);
			*(temp_array + i) = temp_order;
		}
	}

	else if (!increasing){
		for (int i = 0; i < QUEUE_SIZE; i++){
			index = i;
			temp_order = *(temp_array + i);
			for (int j = i; j < QUEUE_SIZE; j++){
				if ( ((temp_array + j)->activeOrder) && ((temp_array + j)->floor > temp_order.floor) ){
					temp_order = *(temp_array + j);
					index = j;
				}
			}
			*(temp_array + index) = *(temp_array + i);
			*(temp_array + i) = temp_order;
		}
	}

}


void orderQueue_sortChunksByDirection(int countUp,int countDown,int countSecondUp,int countSecondDown,HardwareMovement direction){

	orderQueue_sortIncrementally(goingUp, true); //sorting incrementally
	orderQueue_sortIncrementally(goingDown, false); //sorting decrementally
	orderQueue_sortIncrementally(secondGoingUp, true); //sorting incrementally
	orderQueue_sortIncrementally(secondGoingDown, false); //sorting decrementally

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
			
			if (countSecondDown != 0) {
				printf("ERROR: faulty floor order");
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

			if (countSecondUp != 0) {
				printf("ERROR: faulty floor order");
			}
			break;

		case HARDWARE_MOVEMENT_STOP:

			printf("ERROR: logic error, movement stop should not happen here: will sort as if going down.");

			break;
	}
	for (int i = 0; i < QUEUE_SIZE; i++){
		goingUp[i] = EMPTYORDER;
		goingDown[i] = EMPTYORDER;
		secondGoingUp[i] = EMPTYORDER;
		secondGoingDown[i] = EMPTYORDER;
	}
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

					if (atFloor){
						//everything bigger, _including_ current floor
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
					} else {
						//everything bigger, _not including_ current floor (because elevator is inbetween floors, and current floor isn't representative)
						if ( (order_floor > elevator_floor) && (order_type == HARDWARE_ORDER_UP || order_type == HARDWARE_ORDER_INSIDE) ){
							goingUp[countUp] = orderQueue[i];
							countUp++;
						}
						else if ( (order_type == HARDWARE_ORDER_DOWN) || ((order_floor <= elevator_floor) && order_type == HARDWARE_ORDER_INSIDE) ){
							goingDown[countDown] = orderQueue[i];
							countDown++;
						}
						else if ( (order_floor <= elevator_floor) && (order_type == HARDWARE_ORDER_UP) ){
							secondGoingUp[countSecondUp] = orderQueue[i];
							countSecondUp++;
						}
					}

					break;
				case HARDWARE_ORDER_DOWN:

					if (atFloor){
						//everything lesser, _including_ current floor
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
					} else {
						//everything lesser, _not including_ current floor (because elevator is inbetween floors, and current floor isn't representative)
						if ( (order_floor < elevator_floor) && (order_type == HARDWARE_ORDER_DOWN || order_type == HARDWARE_ORDER_INSIDE) ){
							goingDown[countDown] = orderQueue[i];
							countDown++;
						}
						else if ( (order_type == HARDWARE_ORDER_UP) || ((order_floor >= elevator_floor) && order_type == HARDWARE_ORDER_INSIDE) ){
							goingUp[countUp] = orderQueue[i];
							countUp++;
						}
						else if ( (order_floor >= elevator_floor) && (order_type == HARDWARE_ORDER_DOWN) ){
							secondGoingDown[countSecondDown] = orderQueue[i];
							countSecondDown++;
						}
					}

					break;
				case HARDWARE_MOVEMENT_STOP:

					printf("ERROR: logic error, movement stop should not happen here: will sort as if going down.");

					break;
			}
		}
	}
	orderQueue_sortChunksByDirection(countUp, countDown, countSecondUp, countSecondDown, direction);
}

int orderQueue_addOrder(Order order, int elevator_floor, HardwareMovement direction){
	orderQueue[QUEUE_SIZE-1] = order_copy(order);
	orderQueue_sortOrderQueue(elevator_floor, direction);
	orderQueue[QUEUE_SIZE-1] = order_delete(orderQueue[QUEUE_SIZE-1]);
	return 0;
}
