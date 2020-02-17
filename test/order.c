#include "order.h"

struct Order order_init(int floor, HardwareOrder order_type) {
    struct Order order;
    order.floor = floor;
    order.order_type = order_type;
    order.emptyOrder = false;
    hardware_command_order_light(floor,order_type,1);
    return order;
}

void order_turn_off_light(struct Order order) {
    hardware_command_order_light(order.floor, order.order_type, 0);
}

struct Order order_make_empty(struct Order order) {
    order.emptyOrder = true;
    return order;
}

struct Order order_delete(struct Order order) {
    order_turn_off_light(order);
    order = order_make_empty(order);
    return order;
}