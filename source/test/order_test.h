/**
 * @file
 * @brief Order queue functionality
 *
 */

#include "hardware_test.h"
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
 * @brief Deletes order by deactivating and turning off order light.
 *
 * @return returns deleted order.
 */
Order order_delete(Order order);

/**
 * @brief Copies the input order
 *
 * @return returns the copy
 */
Order order_copy(Order order);