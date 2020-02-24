#include "order.h"

int orderTable[4][3];

Order order_init(int floor, HardwareOrder order_type) {
    Order order;
    order.floor = floor;
    order.order_type = order_type;
    order.activeOrder = true;
    hardware_command_order_light(floor,order_type,1);
    return order;
}

void order_turn_off_light(Order order) {
    hardware_command_order_light(order.floor, order.order_type, 0);
}

Order order_delete(Order order) {
    order_toggle_unique(order, 0);
    order_turn_off_light(order);
    return EMPTYORDER;
}

Order order_copy(Order order) {
    Order orderCopy = {order.floor, order.order_type, order.activeOrder};
    return orderCopy;
}

void order_toggle_unique(Order order, int on) {
    if (on) {
        orderTable[order.floor][order.order_type] = 1; 
    } else {
        orderTable[order.floor][order.order_type] = 0; 
    }
}

int order_check_unique(Order order) {
    if (!orderTable[order.floor][order.order_type]) {
        return 1;
    } else {
        return 0;
    }
}