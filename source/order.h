/**
 * @file
 * @brief Order queue functionality
 *
 */

#include "hardware.h"
#include <stdbool.h>

typedef struct{

    int floor;
    HardwareOrder order_type;
    bool emptyOrder;

}Order;

/**
 * @brief Initializes valid order.
 *
 * @return 0 on success. Non-zero for failure.
 */
Order order_init(int floor, HardwareOrder order_type);

/**
 * @brief Turns off corresponing light.
 *
 */
void order_turn_off_light(Order order);

/**
 * @brief Deactivates order by setting emptyOrder to true.
 *
 * @return returns deactivated order.
 */
Order order_make_empty(Order order);

/**
 * @brief Deletes order by deactivating and turning off order light.
 *
 * @return returns deleted order.
 */
Order order_delete(Order order);

/**
 * @brief returns a copy of the input order.
 *
 * @return returns order of Order type.
 */
Order order_copy(Order order);
