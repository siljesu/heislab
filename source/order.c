#include "order.h"

int orderTable[4][3];

Order order_init(int floor, HardwareOrder orderType) {

    Order order;
    order.floor = floor;
    order.order_type = orderType;
    order.activeOrder = true;
    hardware_command_order_light(floor,orderType,1);
    return order;
}

void order_turnOffLight(Order order) {
    hardware_command_order_light(order.floor, order.order_type, 0);
}

Order order_delete(Order order) {
    order_toggleUnique(order, 0);
    order_turnOffLight(order);
    return EMPTYORDER;
}

Order order_copy(Order order) {
    Order orderCopy = {order.floor, order.order_type, order.activeOrder};
    return orderCopy;
}

void order_toggleUnique(Order order, int on) {
    if (on) {
        orderTable[order.floor][order.order_type] = 1; 
    } else {
        orderTable[order.floor][order.order_type] = 0; 
    }
}

int order_checkUnique(Order order) {
    if (!orderTable[order.floor][order.order_type]) {
        return 1;
    } else {
        return 0;
    }
}