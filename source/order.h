/**
 * @file
 * @brief order functionality
 */

#include "hardware.h"
#include <stdbool.h>

/**
 * @brief Lookup table that shows "true" (1) for each order that's placed. 
 * Table's layout corresponds with hardware.c, lines 127 - 132.
 * Used in @c order_toggleUnique and @c order_checkUnique
 */
extern int orderTable[4][3];

/**
 * @brief Type used to store orders received.
 */
typedef struct{

    int floor; /**< The floor the order comes from or is to*/
    HardwareOrder order_type; /**< The order's order_type; up or down from hall, or from inside the cab*/
    bool activeOrder; /**< Whether the order is active or not*/

}Order;

/**
 * @brief A standard empty order to initialize arrays.
 */
static const Order EMPTYORDER = {0,HARDWARE_ORDER_UP,false};


/**
 * @brief Initializes valid order.
 * 
 * @param floor The floor the order is being made from or to.
 * 
 * @param orderType The type of order; up or down from hall, or from inside the cab.
 *
 * @return The initialized order.
 */
Order order_init(int floor, HardwareOrder orderType);

/**
 * @brief Deletes order by deactivating its bit in orderTable (see above), 
 * and turning off order light.
 * 
 * @param order The order that is to be deleted.
 *
 * @return @c EMPTYORDER (see above).
 */
Order order_delete(Order order);

/**
 * @brief Copies the input order.
 * 
 * @param order The order that is to be copied
 *
 * @return the copy; @p order.
 */
Order order_copy(Order order);

/**
 * @brief Toggles corresponding bit in orderTable to 1 if unique, 0 if removing order.
 * 
 * @param order The order whose bit in orderTable will be toggled.
 * 
 * @param on 1 to set the bit (to 1), 0 to remove the bit (set to 0)
 */
void order_toggleUnique(Order order, int on);

/**
 * @brief Checks if order already exists.
 * 
 * @param order The order that will be checked for uniqueness (whether it already exists or not).
 *
 * @return 1 if @p order is unique, 0 if already exists.
 */
int order_checkUnique(Order order);