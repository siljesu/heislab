/**
 * @file
 * @brief Order queue functionality
 */

#include "hardware.h"
#include <stdbool.h>

/**
 * @brief Lookup table that shows "true" (1) for each order that's placed. Orders correspond with hardware.c, lines 127 - 132
 */
extern int orderTable[4][3];

/**
 * @brief Type used in the array OrderQueue (see orderQueue.h), which in turn is used to keep track of all current orders.
 */
typedef struct{

    int floor;
    HardwareOrder order_type;
    bool activeOrder;

}Order;

/**
 * @brief A standard empty order to initialize arrays.
 */
static const Order EMPTYORDER = {0,HARDWARE_ORDER_UP,false};


/**
 * @brief Initializes valid order.
 *
 * @return The initialized order.
 */
Order order_init(int floor, HardwareOrder orderType);

/**
 * @brief Deletes order by deactivating its bit in orderTable (see aboce), and turning off order light.
 *
 * @return EMPTYORDER (see above).
 */
Order order_delete(Order order);

/**
 * @brief Copies the input order.
 *
 * @return returns the copy.
 */
Order order_copy(Order order);

/**
 * @brief Toggles corresponding bit in orderTable to 1 if unique, 0 if removing order.
 */
void order_toggleUnique(Order order, int on);

/**
 * @brief Checks if order already exists.
 *
 * @return returns 1 if input order is unique, 0 if already exists.
 */
int order_checkUnique(Order order);