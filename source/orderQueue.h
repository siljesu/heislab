/**
 * @file
 * @brief Order queue functionality
 *
 */

#include <stdbool.h> //Exists in order.h. Remove once merged properly
#include "order.h"


/**
 * @brief Table to hold up to 12 orders (12 is the max possible), sorted after proximity in the elevator's current path.
 */
Order orderQueue[12];

Order goingUp[12];
Order goingDown[12];
Order secondGoingUp[12]; // happens only if already going up
Order secondGoingDown[12]; // happens only if already going down

/**
 * @brief Clears whole queue.
 */
void orderQueue_clear();


/**
 * @brief Checks if ordering queue is empty.
 *
 * @return 1 if empty
 * @return 0 if not empty
*/
int orderQueue_empty();

/**
 * @brief shifts every element in the order queue one place towards left, from the index given
 */
void orderQueue_deleteByShiftingAtIndex(int i);

/**
 * @brief Sorts queue, must be used when adding order.
 */
void orderQueue_sortIncrementally(Order* temp_array, bool increasing);

 /** 
 * @brief Receives "chunks" of sorted arrays, divided by direction. Sorts these in sequence, based on direction.
 */
void orderQueue_sortChunksByDirection(int count_up,int count_down,int count_second_up,int count_second_down,HardwareMovement direction);

/**
 * @brief Sorts entire order queue, based on direction
 */
void orderQueue_sortOrderQueue(int elevator_floor, HardwareMovement direction);

/**
 * @brief Adds a single, valid order
 *
 * @return 1 if true, 0 if false.
 */
int orderQueue_addOrder(Order* order, int elevator_floor, HardwareMovement direction);

