/**
 * @file
 * @brief Order queue functionality
 *
 */

#include "hardware.h"
#include <stdbool.h>

struct Order {

    int floor;
    HardwareOrder order_type;
    bool emptyOrder;

};

/**
 * @brief Initializes valid order.
 *
 * @return 0 on success. Non-zero for failure.
 */
struct Order order_init(int floor, HardwareOrder order_type);

/**
 * @brief Turns off corresponing light.
 *
 */
void order_turn_off_light(struct Order order);

/**
 * @brief Deactivates order by setting emptyOrder to true.
 *
 * @return returns deactivated order.
 */
struct Order order_make_empty(struct Order order);

/**
 * @brief Deletes order by deactivating and turning off order light.
 *
 * @return returns deleted order.
 */
struct Order order_delete(struct Order order);