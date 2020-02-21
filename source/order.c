#include "order.h"


Order order_init(int floor, HardwareOrder order_type) {
    Order order;
    order.floor = floor;
    order.order_type = order_type;
    order.emptyOrder = false;
    hardware_command_order_light(floor,order_type,1);
    return order;
}

void order_turn_off_light(Order order) {
    hardware_command_order_light(order.floor, order.order_type, 0);
}

Order order_delete(Order order) {
    order_turn_off_light(order);
    return EMPTYORDER;
}

Order order_copy(Order order) {
    Order orderCopy = {order.floor, order.order_type, order.emptyOrder};
    return orderCopy;
}

void order_toggle_unique(Order order, int on) {
    if (on) {
        order_table[order.floor][order.order_type] = 1; 
    } else {
        order_table[order.floor][order.order_type] = 0; 
    }
}

int order_check_unique(Order order) {
    if (!order_table[order.floor][order.order_type]) {
        return 1;
    } else {
        return 0;
    }
}