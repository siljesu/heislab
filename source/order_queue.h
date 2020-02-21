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
Order order_queue[12];

/**
 * @brief Arrays for sorting the queue.
 */
Order going_up[12];
Order going_down[12];
Order second_going_up[12]; // happens only if already going up
Order second_going_down[12]; // happens only if already going down


/**
 * @brief Clears whole queue.
 */
void order_queue_clear();


/**
 * @brief Checks if ordering queue is empty.
 *
 * @return 1 if empty
 * @return 0 if not empty
*/
int order_queue_empty();

/**
 * @brief shifts every element in the order queue one place towards left
 */
void order_queue_deleteByShifting();

/**
 * @brief Sorts queue, must be used when adding order.
 */
void order_queue_sort_incrementally(Order* temp_array, bool increasing);

 /** 
 * @brief Receives "chunks" of sorted arrays, divided by direction. Sorts these in sequence, based on direction.
 */
void order_queue_sortChunksByDirection(Order* going_up,Order* going_down,Order* second_going_up,Order* second_going_down,int count_up,int count_down,int count_second_up,int count_second_down,int elevator_floor,HardwareMovement direction);

/**
 * @brief Sorts entire order queue, based on direction
 */
void order_queue_sortOrderQueue(int elevator_floor, HardwareMovement direction);

/**
 * @brief Adds a single, valid order
 *
 * @return 1 if true, 0 if false.
 */
int order_queue_add_order(Order* order, int elevator_floor, HardwareMovement direction);

