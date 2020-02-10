#include <stdio.h> //undersøk om nødvendig å inkludere
#include "order_queue.h"

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
		for (int i = 0; i < (QUEUE_SIZE - 1); i++){
			if ( (!*(temp_array + i + 1).emptyOrder) && (*(temp_array + i + 1).floor <= *(temp_array + i).floor) ){ //=> emptyOrder == false will go into if-statement
				temp_order = *(temp_array + i);
				*(temp_array + i) = *(temp_array + i + 1)
				*(temp_array + i + 1) = temp_order;
			}
		}
	}
	else if (!increasing){
		for (int i = 0; i < (QUEUE_SIZE - 1); i++){
			if ( (!*(temp_array + i + 1).emptyOrder) && (*(temp_array + i + 1).floor >= *(temp_array + i).floor) ){ //=> emptyOrder == false will go into if-statement
				temp_order = *(temp_array + i);
				*(temp_array + i) = *(temp_array + i + 1)
				*(temp_array + i + 1) = temp_order;
			}
		}
	}
} //Only does one step for every element, even though every element may require many. Needs filling out.

#define MAX_POSSIBLE_IN_ONE_DIRECTION 8

int order_queue_add_order(struct Order* order, int elevator_floor, HardwareMovement direction){
	//struct Order sorted_by_direction[QUEUE_SIZE];
	struct Order going_up[QUEUE_SIZE;
	struct Order going_down[QUEUE_SIZE];

	int count_up = 0;
	int count_down = 0;

	switch(direction):
		
		case HARDWARE_MOVEMENT_UP:
			for (int i = 0; i < (QUEUE_SIZE - 1); i++){

				if ( (*(order + i).floor >= elevator_floor ) 
				&& (*(order + i).order_type == (HARDWARE_ORDER_UP || HARDWARE_ORDER_INSIDE)) ){//Covers all orders to be adressed on the way up
					going_up[count_up] = *(order + i);
					count_up++;
				}

				else {
					going_down[count_down] = *(order + i);
					count_down++;
				}
			}

			order_queue_sort_incrementally(going_up, true); //sorting incrementally
			order_queue_sort_incrementally(going_down, false); //sorting decrementally

			for (int i = 0; i < count_up; i++){
				order_queue[i] = going_up[i];
			}
			for (int i = count_down; i < (count_up + count_down); i++){
				order_queue[i] = going_down[i];
			}
			for (int i = (count_up + count_down); i < (QUEUE_SIZE - 1); i++){
				order_queue[i]->emptyOrder = true;
			}
			break;
		
		case HARDWARE_MOVEMENT_DOWN:
			for (int i = 0; i < (QUEUE_SIZE - 1); i++){

				if ( (*(order + i).floor <= elevator_floor ) 
				&& (*(order + i).order_type == (HARDWARE_ORDER_DOWN || HARDWARE_ORDER_INSIDE)) ){
					going_down[count_down] = *(order + i);
					count_down++;
				}
				else {
					going_up[count_up] = *(order + i);
					count_up++;
				}
			}

			order_queue_sort_incrementally(going_up, true); //sorting incrementally
			order_queue_sort_incrementally(going_down, false); //sorting decrementally

			for (int i = 0; i < count_down; i++){
				order_queue[i] = going_down[i];
			}
			for (int i = count_down; i < (count_down + count_up); i++){
				order_queue[i] = going_up[i];
			}
			for (int i = (count_down + count_up); i < (QUEUE_SIZE - 1); i++){
				order_queue[i]->emptyOrder = true;
			}
			break;

		case HARDWARE_MOVEMENT_STOP:
			//Queue should already be cleared if elevator has stopped. Consequently, the order placed is the only possible one.
			order_queue[0] = *order;
			break;
/*
	int priority;
	switch (direction):
		case HARDWARE_MOVEMENT_UP:
			priority = 1; //1 for up
			break;
		case HARDWARE_MOVEMENT_DOWN:
			priority = 0; //0 for down
			break;
		case HARDWARE_MOVEMENT_STOP:
			break;

	int count_same = 0;
	int count_opposite = QUEUE_SIZE - 1;
	//Separating in going up/going down
	if (priority){

	}
*/
/*
		for (int i = 0; i < (QUEUE_SIZE - 1); i++){

			if ((*(order + i).floor > elevator_floor) && (*(order + i).emptyOrder == false)){
				sorted_by_direction[count_same] = 
				//same_direction[count_same] = *(order + i);
				//count_same++;
			}
			
			else if ((*(order + i).floor < elevator_floor) && (*(order + i).emptyOrder == false)){
				opposite_direction[count_opposite] = *(order + i);
				count_opposite++;
			}
		}
*/
	//}
	//for (int i = (QUEUE_SIZE - 1) * priority; i < (QUEUE_SIZE - 1) - (QUEUE_SIZE - 1)*priority; i++){

	//}
/*
	for (int i = 0; i < QUEUE_SIZE; i++){
		if (*(order + i).emptyOrder == false){
			if ((*(order + i).floor > elevator_floor) && (*(order + i).order_type == HARDWARE_MOVEMENT_UP)){
				going_up[i] = *(order + i);	
			}
			else if ((*(order + i).floor > elevator_floor) && (*(order + i).order_type == HARDWARE_MOVEMENT_UP))
		}
	}
*/
	//Going up, prioritér: 		current etasje - bestillingsetasje < 0
	//Going down, prioritér: 	current etasje - bestillingsetasje > 0
	//Sortér begge
	//=> hvilket array som feedes inn først i rekkefølge

	
}