#include <stdio.h> //undersøk om nødvendig å inkludere
#include "order_queue.h"

int QUEUE_SIZE = 12;

void order_queue_clear(){
	for (int i = 0; i < QUEUE_SIZE ; i++){
		order_delete(order_queue[i]);
	}
}

void order_queue_shift(){
	order_turn_off_light(order_queue[0]);
	for (int i = 0; i < (QUEUE_SIZE - 1); i++){
		order_queue[i].floor = order_queue[i + 1].floor;
		order_queue[i].order_type = order_queue[i + 1].order_type;
		order_queue[i].emptyOrder = order_queue[i + 1].emptyOrder;
	}
	order_queue[QUEUE_SIZE - 1] = order_make_empty(order_queue[QUEUE_SIZE - 1]);
}

void order_queue_sort_incrementally(struct Order* temp_array, bool increasing){
	struct Order temp_order;
	
	if (increasing){
		temp_order = *(temp_array);
		for (int i = 0; i < (QUEUE_SIZE - 2); i++){
			for (int j = i; j < (QUEUE_SIZE - 2); j++){
				if ( (!(temp_array + j + 1)->emptyOrder) && ((temp_array + j + 1)->floor <= temp_order.floor) ){ //=> emptyOrder == false will go into if-statement
					temp_order = *(temp_array + j + 1);
				}	
			}
			*(temp_array + i) = temp_order;
		}
	}

	else if (!increasing){
		temp_order = *(temp_array);
		for (int i = 0; i < (QUEUE_SIZE - 2); i++){
			for (int j = i; j < (QUEUE_SIZE - 2); j++){
				if ( (!(temp_array + j + 1)->emptyOrder) && ((temp_array + j + 1)->floor >= temp_order.floor) ){ //=> emptyOrder == false will go into if-statement
					temp_order = *(temp_array + j + 1);
				}	
			}
			*(temp_array + i) = temp_order;
		}
	}

}

//ADD FUNCTIONALITY TO MAKE SURE LAST ORDERS ARE EMPTY ORDERS
void order_queue_sortChunksByDirection(struct Order* going_up, 
										struct Order* going_down, struct Order* other,
										int count_up, int count_down, int count_other, 
										int elevator_floor, HardwareMovement direction){

	order_queue_sort_incrementally(going_up, true); //sorting incrementally
	order_queue_sort_incrementally(going_down, false); //sorting decrementally

/*
	if(HardwareMovement == HARDWARE_MOVEMENT_STOP){
		if (order_queue->floor > elevator_floor){
			direction = HARDWARE_MOVEMENT_UP;
		}
		else if (order_queue->floor < elevator_floor){
			direction = HARDWARE_MOVEMENT_DOWN;
		}
	}
*/

	switch(direction) {
		case HARDWARE_MOVEMENT_UP:

			order_queue_sort_incrementally(other, true); //sorting incrementally

			for (int i = 0; i < count_up; i++){
				order_queue[i] = going_up[i];
			}
			for (int i = count_down; i < (count_up + count_down); i++){
				order_queue[i] = going_down[i - count_up];
			}
			for (int i = (count_up + count_down); i < (count_up + count_down + count_other); i++){
				order_queue[i] = other[i - (count_up + count_down)];
			}
			for (int i = (count_up + count_down + count_other); i < (QUEUE_SIZE - 1); i++){
				order_queue[i].emptyOrder = true;
			}
			break;
		
		case HARDWARE_MOVEMENT_DOWN:

			order_queue_sort_incrementally(other, false); //sorting decrementally

			for (int i = 0; i < count_down; i++){
				order_queue[i] = going_down[i];
			}
			for (int i = count_down; i < (count_down + count_up); i++){
				order_queue[i] = going_up[i - count_down];
			}
			for (int i = (count_down + count_up); i < (count_down + count_up + count_other); i++){
				order_queue[i] = other[i - (count_up + count_down)];
			}
			for (int i = (count_down + count_up + count_other); i < (QUEUE_SIZE - 1); i++){
				order_queue[i].emptyOrder = true;
			}
			break;

		case HARDWARE_MOVEMENT_STOP:
			//error; //NEEDS FILLING OUT
			break;


//		case HARDWARE_MOVEMENT_STOP:
//			//Queue should already be cleared if elevator has stopped. Consequently, the order placed is the only possible one.
//			order_queue[0] = *order;
//			break;
	}
}

void order_queue_sortOrderQueue(int elevator_floor, HardwareMovement direction){

	struct Order going_up[QUEUE_SIZE]; //Set to QUEUE_SIZE for redundancy, rather than limitation
	struct Order going_down[QUEUE_SIZE];
	struct Order other[QUEUE_SIZE];

	int count_up = 0;
	int count_down = 0;
	int count_other = 0;

	for (int i = 0; i < (QUEUE_SIZE - 1); i++){
		if ((order_queue + i)->emptyOrder == false){
			if ( ((order_queue + i)->floor >= elevator_floor ) 
			&& ((order_queue + i)->order_type == (HARDWARE_ORDER_UP || HARDWARE_ORDER_INSIDE)) ){//Covers all orders to be adressed on the way up
				going_up[count_up] = *(order_queue + i);
				count_up++;
			}

			else if ( ((order_queue + i)->floor <= elevator_floor ) 
			&& ((order_queue + i)->order_type == (HARDWARE_ORDER_DOWN || HARDWARE_ORDER_INSIDE)) ){
				going_down[count_down] = *(order_queue + i);
				count_down++;
			}

			else {
				other[count_other] = *(order_queue + i);
				count_other++;
			}
		}
	}
	order_queue_sortChunksByDirection(going_up, going_down, other, count_up, count_down, count_other, elevator_floor, direction);
}


int order_queue_add_order(struct Order* order, int elevator_floor, HardwareMovement direction){
	//Adds current order at _end_ of queue, before sorting
	*(order_queue + (QUEUE_SIZE - 1)) = *order;

	order_queue_sortOrderQueue(elevator_floor, direction);
	return 0;
}

int order_queue_empty(){
	for (int i = 0; i < (QUEUE_SIZE - 1); i++){
		if (!order_queue[i].emptyOrder)
			return 0;
	}
	return 1;
}