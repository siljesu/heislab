/**
 * @file
 * @brief Order queue functionality
 *
 */

#include "hardware.h"
#include <stdbool.h>

/**
 * @brief A standard empty order to initialize arrays.
 */
Order EMPTYORDER = {0,HARDWARE_MOVEMENT_UP,true};

/**
 * @brief Lookup table that shows "true" (1) for each order that's placed. Orders correspond with hardware.c, lines 127 - 132
 */
int order_table[4][3];

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

/**
 * @brief Toggles corresponding order in order_table to 1 if unique, 0 if removing order.
 *
 * @return none
 */
void order_toggle_unique(Order order, int on);

/**
 * @brief Checks if order already exists.
 *
 * @return returns 1 if unique, 0 if already exists.
 */
int order_check_unique(Order order);