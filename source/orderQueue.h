/**
 * @file
 * @brief Order queue functionality
 *
 */

#include "order.h"

/**
 * @brief Enum type that contains the elevator's possible relative positions, 
 * relative to it's last floor (g_FLOOR, see elevator.h)
 */
typedef enum
{
    BELOW,
    ABOVE,
    AT
} RelativePosition;

/**
 * @brief Global variable that holds the elevator's current relative position.
 */
extern RelativePosition g_relativePosition;

/**
 * @brief Global array to hold up to 12 orders (12 is the max theoretical possible), 
 * sorted after proximity to the elevator's current position, 
 * and according to current direction. Used to keep track of all current orders.
 */
Order orderQueue[12];

/**
 * @brief Array to hold all orders that need to be handled next time elevator is going up. 
 * This can be immediately, but also in second turn after elevator has already gone down, 
 * if its initial direction was down (in accordance with requirements).
 * Used in @c orderQueue_sortOrderQueue and @c orderQueue_sortChunksByDirection.
 */
Order goingUp[12];

/**
 * @brief Array to hold all orders that need to be handled next time elevator is going down. 
 * This can be immediately, but also in second turn after elevator has already gone up, 
 * if its initial direction was up (in accordance with requirements).
 * Used in @c orderQueue_sortOrderQueue and @c orderQueue_sortChunksByDirection.
 */
Order goingDown[12];

/**
 * @brief Array to hold all orders that need to be handled in the elevator's "second round up". 
 * Only used when elevator's direction is up.
 * Used in @c orderQueue_sortOrderQueue and @c orderQueue_sortChunksByDirection.
 */
Order secondGoingUp[12];

/**
 * @brief Array to hold all orders that need to be handled in the elevator's "second round down". 
 * Only used when elevator's direction is down.
 * Used in @c orderQueue_sortOrderQueue and @c orderQueue_sortChunksByDirection.
 */
Order secondGoingDown[12];

/**
 * @brief Clears all orders in queue.
 */
void orderQueue_clear();

/**
 * @brief shifts every element in the order queue one place towards left (towards lower indices), 
 * from the index given.
 * 
 * @param i The index, from which all orderQueue's elements to the right will be left-shifted 
 * (i.e. shifted towards lower indices)
 */
void orderQueue_deleteByShiftingAtIndex(int i);

/**
 * @brief Adds a single, valid order, then sorts entire orderQueue.
 * 
 * @param order The order that is to be added to the queue.
 * 
 * @param elevator_floor The elevator's current floor upon request to add the order. 
 * 
 * @param direction The elevator's current direction upon request to add the order.
 */
void orderQueue_addOrder(Order order, int elevator_floor, HardwareMovement direction);