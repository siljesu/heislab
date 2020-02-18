#include <stdio.h> //undersøk om nødvendig å inkludere
#include "new_order_queue_test.h"

int QUEUE_SIZE = 12;

void order_queue_clear(){
	for (int i = 0; i < QUEUE_SIZE ; i++){
		Order tmpOrder = order_copy(order_delete(order_queue[i]));
		order_queue[i] = order_copy(tmpOrder);
	}
}

int order_queue_empty(){
	for (int i = 0; i < (QUEUE_SIZE - 1); i++){
		if (!order_queue[i].emptyOrder)
			return 0;
	}
	return 1;
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

void order_queue_sort_incrementally(Order* temp_array, bool increasing){
	Order temp_order;
	int index;

	if (increasing){
		for (int i = 0; i < QUEUE_SIZE; i++){
			index = i;
			temp_order = *(temp_array + i);
			for (int j = i; j < QUEUE_SIZE; j++){
				if ( (!(temp_array + j)->emptyOrder) && ((temp_array + j)->floor < temp_order.floor) ){ //=> emptyOrder == false will go into if-statement
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
				if ( (!(temp_array + j)->emptyOrder) && ((temp_array + j)->floor > temp_order.floor) ){ //=> emptyOrder == false will go into if-statement
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
void order_queue_sortChunksByDirection(Order* going_up,Order* going_down,Order* second_going_up,Order* second_going_down,int count_up,int count_down,int count_second_up,int count_second_down,int elevator_floor,HardwareMovement direction){

	order_queue_sort_incrementally(going_up, true); //sorting incrementally
	order_queue_sort_incrementally(going_down, false); //sorting decrementally
	order_queue_sort_incrementally(second_going_up, true); //sorting incrementally
	order_queue_sort_incrementally(second_going_down, false); //sorting decrementally
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
    int end_border;

	switch(direction) {
		case HARDWARE_MOVEMENT_UP:

			end_border = count_up;

			for (int i = offset; i < end_border; i++){
				order_queue[i] = going_up[i - offset];
			}

			offset += count_down;
			end_border += count_down;
			for (int i = offset; i < end_border; i++){
				order_queue[i] = going_down[i - offset];
			}
			
			offset += count_second_up;
			end_border += count_second_up;
			for (int i = offset; i < end_border; i++){
				order_queue[i] = second_going_up[i - offset];
			}
			
			if (count_second_down != 0) {
				printf("ERROR: faulty floor order");
			}
			break;
		
		case HARDWARE_MOVEMENT_DOWN:

			end_border = count_down;

			for (int i = offset; i < end_border; i++){
				order_queue[i] = going_down[i - offset];
			}

			offset += count_down;
			end_border += count_up;
			for (int i = offset; i < end_border; i++){
				order_queue[i] = going_up[i - offset];
			}
			
			offset += count_up;
			end_border += count_second_down;
			for (int i = offset; i < end_border; i++){
				order_queue[i] = second_going_down[i - offset];
			}
			
			if (count_second_up != 0) {
				printf("ERROR: faulty floor order");
			}
			break;

		case HARDWARE_MOVEMENT_STOP:

			printf("ERROR: logic error, movement stop should not happen here: will sort as if going down.");

			//choosing movement down scenario just in case.
			end_border = count_down;

			for (int i = offset; i < end_border; i++){
				order_queue[i] = going_down[i - offset];
			}

			offset += count_up;
			end_border += count_up;
			for (int i = offset; i < end_border; i++){
				order_queue[i] = going_up[i - offset];
			}
			
			offset += count_second_down;
			end_border += count_second_down;
			for (int i = offset; i < end_border; i++){
				order_queue[i] = second_going_down[i - offset];
			}
			
			if (count_second_up != 0) {
				printf("ERROR: faulty floor order");
			}
			break;

//		case HARDWARE_MOVEMENT_STOP:
//			//Queue should already be cleared if elevator has stopped. Consequently, the order placed is the only possible one.
//			order_queue[0] = *order;
//			break;
	}
}

void order_queue_sortOrderQueue(int elevator_floor, HardwareMovement direction){

	int count_up = 0;
	int count_down = 0;
	int count_second_up = 0;
	int count_second_down = 0;

	for (int i = 0; i < (QUEUE_SIZE); i++){
		if ((order_queue + i)->emptyOrder == false){

			switch (direction) {
				case HARDWARE_MOVEMENT_UP:

					if (((order_queue + i)->floor >= elevator_floor)&&((order_queue + i)->order_type == HARDWARE_ORDER_UP || (order_queue + i)->order_type == HARDWARE_ORDER_INSIDE)){
						going_up[count_up] = *(order_queue + i);
						count_up++;
					}
					else if (((order_queue + i)->floor < elevator_floor)&&((order_queue + i)->order_type == HARDWARE_ORDER_DOWN || (order_queue + i)->order_type == HARDWARE_ORDER_INSIDE) || ((order_queue + i)->floor > elevator_floor)&&((order_queue + i)->order_type == HARDWARE_ORDER_DOWN)){
						going_down[count_down] = *(order_queue + i);
						count_down++;
					}
					else if (((order_queue + i)->floor < elevator_floor)&&((order_queue + i)->order_type == HARDWARE_ORDER_UP || (order_queue + i)->order_type == HARDWARE_ORDER_INSIDE)){
						second_going_up[count_down] = *(order_queue + i);
						count_second_up++;
					}
					break;
				case HARDWARE_ORDER_DOWN:
					if (((order_queue + i)->floor > elevator_floor)&&((order_queue + i)->order_type == HARDWARE_ORDER_UP || (order_queue + i)->order_type == HARDWARE_ORDER_INSIDE) || ((order_queue + i)->floor < elevator_floor)&&((order_queue + i)->order_type == HARDWARE_ORDER_UP)){
						going_up[count_up] = *(order_queue + i);
						count_up++;
					}
					else if (((order_queue + i)->floor <= elevator_floor)&&((order_queue + i)->order_type == HARDWARE_ORDER_DOWN || (order_queue + i)->order_type == HARDWARE_ORDER_INSIDE)){
						going_down[count_down] = *(order_queue + i);
						count_down++;
					}
					else if (((order_queue + i)->floor > elevator_floor)&&((order_queue + i)->order_type == HARDWARE_ORDER_DOWN ||(order_queue + i)->order_type == HARDWARE_ORDER_INSIDE)){
						second_going_down[count_down] = *(order_queue + i);
						count_second_down++;
					}
					break;
			}
		}
	}
	order_queue_sortChunksByDirection(going_up, going_down, second_going_up, second_going_down, count_up, count_down, count_second_up, count_second_down, elevator_floor, direction);
}


int order_queue_add_order(Order* order, int elevator_floor, HardwareMovement direction){
	order_queue[QUEUE_SIZE-1] = order_copy(*order);
	order_queue_sortOrderQueue(elevator_floor, direction);
	//need to delete added and sorted order. how?
	order_queue[QUEUE_SIZE-1] = emptyOrder;
	return 0;
}


int main(){

	for (int i = 0; i < 12; i++) {
		order_queue[i] = order_copy(emptyOrder);
	}
	

	for (int i = 0; i < QUEUE_SIZE; i++){
		going_up[i] = order_copy(emptyOrder);
		going_down[i] = order_copy(emptyOrder);
		second_going_up[i] = order_copy(emptyOrder);
		second_going_down[i] = order_copy(emptyOrder);
	}

	p_testOrder->floor = 4;
	p_testOrder->order_type = HARDWARE_ORDER_DOWN;
	p_testOrder->emptyOrder = false;
	order_queue_add_order(p_testOrder, 3, HARDWARE_MOVEMENT_DOWN);

	p_testOrder2->floor = 1;
	p_testOrder2->order_type = HARDWARE_ORDER_UP;
	p_testOrder2->emptyOrder = false;
	order_queue_add_order(p_testOrder2, 3, HARDWARE_MOVEMENT_DOWN);

	p_testOrder3->floor = 3;
	p_testOrder3->order_type = HARDWARE_ORDER_INSIDE;
	p_testOrder3->emptyOrder = false;
	order_queue_add_order(p_testOrder3, 3, HARDWARE_MOVEMENT_DOWN);

	p_testOrder4->floor = 2;
	p_testOrder4->order_type = HARDWARE_ORDER_UP;
	p_testOrder4->emptyOrder = false;
	order_queue_add_order(p_testOrder4, 3, HARDWARE_MOVEMENT_DOWN);

	//((OH NO! THERE IS NO CHECK FOR VALID ORDERS: CORRECTION: IN REAL LIFE THERE IS.))
	//TEST THOROUGHLY NON-STATIC FLOORS!! 
	p_testOrder5->floor = 1;
	p_testOrder5->order_type = HARDWARE_ORDER_INSIDE;
	p_testOrder5->emptyOrder = false;
	order_queue_add_order(p_testOrder5, 3, HARDWARE_MOVEMENT_DOWN);


}