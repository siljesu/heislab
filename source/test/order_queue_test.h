/**
 * @file
 * @brief Order queue functionality
 *
 */
#include <stdbool.h> //Exists in order.h. Remove once merged properly


typedef enum {
    HARDWARE_MOVEMENT_UP,
    HARDWARE_MOVEMENT_STOP,
    HARDWARE_MOVEMENT_DOWN
} HardwareMovement;

typedef enum {
    HARDWARE_ORDER_UP,
    HARDWARE_ORDER_INSIDE,
    HARDWARE_ORDER_DOWN
} HardwareOrder;

typedef struct {

    int floor;
    HardwareOrder order_type;
    bool emptyOrder;

} Order;


Order order_queue[12];
Order testOrder;
Order* p_testOrder = &testOrder;