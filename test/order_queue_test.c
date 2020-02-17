//File for testing order_queue.c-functionality

#include <stdio.h> //undersøk om nødvendig å inkludere
#include "order_queue_test.h"

int QUEUE_SIZE = 12;


void order_queue_sort_incrementally(Order* temp_array, bool increasing){
	Order temp_order;
	
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
void order_queue_sortChunksByDirection(Order* front, Order* going_up, 
										Order* going_down, Order* other,
										int count_front, int count_up, int count_down, int count_other, 
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
	int offset = 0;
	int end_border = count_front;

	for (int i = 0; i < end_border; i++){
		order_queue[i] = front[i];
	}

	switch(direction) {
		case HARDWARE_MOVEMENT_UP:

			order_queue_sort_incrementally(other, true); //sorting incrementally
			offset = count_front;
			end_border += count_up;
			for (int i = offset; i < end_border; i++){
				order_queue[i] = going_up[i - offset];
			}
			offset += count_up;
			end_border += count_down;
			for (int i = offset; i < end_border; i++){
				order_queue[i] = going_down[i - offset];
			}
			offset += count_down;
			end_border += count_other;
			for (int i = offset; i < end_border; i++){
				order_queue[i] = other[i - offset];
			}
			offset += count_other;
			end_border = QUEUE_SIZE - 1;
			for (int i = offset; i < end_border; i++){
				order_queue[i].emptyOrder = true;
			}
			break;
		
		case HARDWARE_MOVEMENT_DOWN:

			order_queue_sort_incrementally(other, false); //sorting decrementally
			offset = count_front;
			end_border += count_down;
			for (int i = offset; i < end_border; i++){
				order_queue[i] = going_up[i - offset];
			}
			offset += count_down;
			end_border += count_up;
			for (int i = offset; i < end_border; i++){
				order_queue[i] = going_down[i - offset];
			}
			offset += count_up;
			end_border += count_other;
			for (int i = offset; i < end_border; i++){
				order_queue[i] = other[i - offset];
			}
			offset += count_other;
			end_border = QUEUE_SIZE - 1;
			for (int i = offset; i < end_border; i++){
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

	Order front[QUEUE_SIZE];
	Order going_up[QUEUE_SIZE]; //Set to QUEUE_SIZE for redundancy, rather than limitation
	Order going_down[QUEUE_SIZE];
	Order other[QUEUE_SIZE];

	int count_front = 0;
	int count_up = 0;
	int count_down = 0;
	int count_other = 0;

	for (int i = 0; i < (QUEUE_SIZE - 1); i++){
		if ((order_queue + i)->emptyOrder == false){
			if ((order_queue + i)->floor == elevator_floor){
				front[count_front] = *(order_queue + i);
				count_front++;
			}

			else if ((order_queue + i)->order_type == HARDWARE_ORDER_UP){
				going_up[count_up] = *(order_queue + i);
				count_up++;
			}
			else if (  ((order_queue + i)->order_type == HARDWARE_ORDER_INSIDE) 
					&& ((order_queue + i)->floor > elevator_floor) ){
				going_up[count_up] = *(order_queue + i);
				count_up++;
			}

			else if ((order_queue + i)->order_type == HARDWARE_ORDER_DOWN){
				going_down[count_down] = *(order_queue + i);
			}
			else if (  ((order_queue + i)->order_type == HARDWARE_ORDER_INSIDE) 
					&& ((order_queue + i)->floor < elevator_floor) ){
				going_down[count_down] = *(order_queue + i);
				count_down++;
			}

			else {
				other[count_other] = *(order_queue + i);
				count_other++;
			}
		}
	}
	order_queue_sortChunksByDirection(front, going_up, going_down, other, count_front, count_up, count_down, count_other, elevator_floor, direction);
}


int order_queue_add_order(Order* order, int elevator_floor, HardwareMovement direction){
	//Adds current order at _end_ of queue, before sorting
	*(order_queue + (QUEUE_SIZE - 1)) = *order;

	order_queue_sortOrderQueue(elevator_floor, direction);
	return 0;
}

int order_queue_empty(){
	for (int i = 0; i < (QUEUE_SIZE - 1); i++){
		if (!(order_queue[i].emptyOrder)){
			return 0;
		}
	}
	return 1;
}

int main(){
	
	for (int i = 0; i < 12; i++){
		p_testOrder->floor = i % 4;
		p_testOrder->order_type = i / 4;
		p_testOrder->emptyOrder = false;
		order_queue_add_order(p_testOrder, 2, HARDWARE_MOVEMENT_DOWN);
	}
}