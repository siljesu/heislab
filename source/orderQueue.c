#include <stdio.h> //undersøk om nødvendig å inkludere
#include "orderQueue.h"

int QUEUE_SIZE = 12;

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
	order_delete(orderQueue[i]);
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

//ADD FUNCTIONALITY TO MAKE SURE LAST ORDERS ARE EMPTY ORDERS
void orderQueue_sortChunksByDirection(int count_up,int count_down,int count_second_up,int count_second_down,HardwareMovement direction){

	orderQueue_sortIncrementally(goingUp, true); //sorting incrementally
	orderQueue_sortIncrementally(goingDown, false); //sorting decrementally
	orderQueue_sortIncrementally(secondGoingUp, true); //sorting incrementally
	orderQueue_sortIncrementally(secondGoingDown, false); //sorting decrementally

	int offset = 0;
    int end_border;

	switch(direction) {
		case HARDWARE_MOVEMENT_UP:

			end_border = count_up;

			for (int i = offset; i < end_border; i++){
				orderQueue[i] = goingUp[i - offset];
			}

			offset += count_up;
			end_border += count_down;
			for (int i = offset; i < end_border; i++){
				orderQueue[i] = goingDown[i - offset];
			}
			
			offset += count_down;
			end_border += count_second_up;
			for (int i = offset; i < end_border; i++){
				orderQueue[i] = secondGoingUp[i - offset];
			}
			
			offset += count_second_up; //Extra failsafe
			end_border += count_second_down;
			for (int i = offset; i < end_border; i++){
				orderQueue[i] = secondGoingDown[i - offset];
			}
			
			if (count_second_down != 0) {
				printf("ERROR: faulty floor order");
			}
			break;
		
		case HARDWARE_MOVEMENT_DOWN:

			end_border = count_down;

			for (int i = offset; i < end_border; i++){
				orderQueue[i] = goingDown[i - offset];
			}

			offset += count_down;
			end_border += count_up;
			for (int i = offset; i < end_border; i++){
				orderQueue[i] = goingUp[i - offset];
			}
			
			offset += count_up;
			end_border += count_second_down;
			for (int i = offset; i < end_border; i++){
				orderQueue[i] = secondGoingDown[i - offset];
			}
			
			offset += count_second_down; //Extra failsafe
			end_border += count_second_up;
			for (int i = offset; i < end_border; i++){
				orderQueue[i] = secondGoingUp[i - offset];
			}

			if (count_second_up != 0) {
				printf("ERROR: faulty floor order");
			}
			break;

		case HARDWARE_MOVEMENT_STOP:

			printf("ERROR: logic error, movement stop should not happen here: will sort as if going down.");

			break;
	}
	for (int i = 0; i < 12; i++){ //Hardcoded queuesize value (12)
		goingUp[i] = EMPTYORDER;
		goingDown[i] = EMPTYORDER;
		secondGoingUp[i] = EMPTYORDER;
		secondGoingDown[i] = EMPTYORDER;
	}
}

void orderQueue_sortOrderQueue(int elevator_floor, HardwareMovement direction){

	int count_up = 0;
	int count_down = 0;
	int count_second_up = 0;
	int count_second_down = 0;

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
							goingUp[count_up] = orderQueue[i];
							count_up++;
						}
						else if ( (order_type == HARDWARE_ORDER_DOWN) || ((order_floor < elevator_floor) && order_type == HARDWARE_ORDER_INSIDE) ){
							goingDown[count_down] = orderQueue[i];
							count_down++;
						}
						else if ( (order_floor < elevator_floor) && (order_type == HARDWARE_ORDER_UP) ){
							secondGoingUp[count_second_up] = orderQueue[i];
							count_second_up++;
						}
					} else {
						//everything bigger, _not including_ current floor (because elevator is inbetween floors, and current floor isn't representative)
						if ( (order_floor > elevator_floor) && (order_type == HARDWARE_ORDER_UP || order_type == HARDWARE_ORDER_INSIDE) ){
							goingUp[count_up] = orderQueue[i];
							count_up++;
						}
						else if ( (order_type == HARDWARE_ORDER_DOWN) || ((order_floor <= elevator_floor) && order_type == HARDWARE_ORDER_INSIDE) ){
							goingDown[count_down] = orderQueue[i];
							count_down++;
						}
						else if ( (order_floor <= elevator_floor) && (order_type == HARDWARE_ORDER_UP) ){
							secondGoingUp[count_second_up] = orderQueue[i];
							count_second_up++;
						}
					}

					break;
				case HARDWARE_ORDER_DOWN:

					if (atFloor){
						//everything bigger, _including_ current floor
						if ( (order_floor <= elevator_floor) && (order_type == HARDWARE_ORDER_DOWN || order_type == HARDWARE_ORDER_INSIDE) ){
							goingDown[count_down] = orderQueue[i];
							count_down++;
						}
						else if ( (order_type == HARDWARE_ORDER_UP) || ((order_floor > elevator_floor) && order_type == HARDWARE_ORDER_INSIDE) ){
							goingUp[count_up] = orderQueue[i];
							count_up++;
						}
						else if ( (order_floor > elevator_floor) && (order_type == HARDWARE_ORDER_DOWN) ){
							secondGoingDown[count_second_down] = orderQueue[i];
							count_second_down++;
						}
					} else {
						//everything bigger, _not including_ current floor (because elevator is inbetween floors, and current floor isn't representative)
						if ( (order_floor < elevator_floor) && (order_type == HARDWARE_ORDER_DOWN || order_type == HARDWARE_ORDER_INSIDE) ){
							goingDown[count_down] = orderQueue[i];
							count_down++;
						}
						else if ( (order_type == HARDWARE_ORDER_UP) || ((order_floor >= elevator_floor) && order_type == HARDWARE_ORDER_INSIDE) ){
							goingUp[count_up] = orderQueue[i];
							count_up++;
						}
						else if ( (order_floor >= elevator_floor) && (order_type == HARDWARE_ORDER_DOWN) ){
							secondGoingDown[count_second_down] = orderQueue[i];
							count_second_down++;
						}
					}

					break;
				case HARDWARE_MOVEMENT_STOP:

					printf("ERROR: logic error, movement stop should not happen here: will sort as if going down.");

					break;
			}
		}
	}
	orderQueue_sortChunksByDirection(count_up, count_down, count_second_up, count_second_down, direction);
}

int orderQueue_add_order(Order* order, int elevator_floor, HardwareMovement direction){
	orderQueue[QUEUE_SIZE-1] = order_copy(*order);
	orderQueue_sortOrderQueue(elevator_floor, direction);
	//need to delete added and sorted order. how?
	orderQueue[QUEUE_SIZE-1] = order_delete(orderQueue[QUEUE_SIZE-1]); //= EMPTYORDER;
	return 0;
}
