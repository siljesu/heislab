#include <stdio.h> //undersøk om nødvendig å inkludere
#include "order_queue.h"

void order_queue_clear(){
	for (int i = 0; i < 12; i++){
		order_delete(order_queue[i]);
	}
}

void order_queue_shift(){
	order_turn_off_light(order_queue[0]);
	for (int i = 0; i < 11; i++){
		order_queue[i].floor = order_queue[i + 1].floor;
		order_queue[i].order_type = order_queue[i + 1].order_type;
		order_queue[i].emptyOrder = order_queue[i + 1].emptyOrder;
	}
	order_queue[11] = order_make_empty(order_queue[11]);
}

int order_queue_sort(){
	
}

int order_queue_add_order(struct Order order){
	struct Order orders_going_up[4] = (Order*) malloc(4 * sizeof(order));
	struct Order orders_going_down[4] = (Order*) malloc(4 * sizeof(order));
	struct Order orders_from_inside[4] = (Order*) malloc(4 * sizeof(order));

	free(orders_going_up);
	free(orders_going_down);
	free(orders_from_inside);
}