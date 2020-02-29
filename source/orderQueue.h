/**
 * @file
 * @brief Order queue functionality
 *
 */
#include "order.h"


/**
 * @brief Table to hold up to 12 orders (12 is the max theoretical possible), sorted after proximity in the elevator's current path.
 */
Order orderQueue[12];
extern Order* p_firstOrder;

Order goingUp[12];
Order goingDown[12];
Order secondGoingUp[12]; // happens only if already going up
Order secondGoingDown[12]; // happens only if already going down

/**
 * @brief Clears whole queue.
 */
void orderQueue_clear();

/**
 * @brief shifts every element in the order queue one place towards left, from the index given
 */
void orderQueue_deleteByShiftingAtIndex(int i);

/**
 * @brief Adds a single, valid order
 *
 * @return 1 if true, 0 if false.
 */
int orderQueue_addOrder(Order order, int elevator_floor, HardwareMovement direction);