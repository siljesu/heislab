/**
 * @file
 * @brief Order queue functionality
 *
 */

#include "order.h"
#include <stdbool.h> //Exists in order.h. Remove once merged properly
#define QUEUE_SIZE 12

/**
 * @brief Lookup table that shows "true" (1) for each order that's placed. Orders correspond with hardware.c, lines 127 - 132
 */
bool order_table[12];

/**
 * @brief Table to hold up to 12 orders (12 is the max possible), sorted after proximity in the elevator's current path.
 */
struct Order order_queue[12];

/**
 * @brief Clears whole queue.
 */
void order_queue_clear();

/**
 * @brief shifts every element in the order queue one place towards left
 */
void order_queue_shift();

/**
 * @brief Sorts queue, must be used when adding order.
 */
void order_queue_sort_incrementally(struct Order* temp_array, bool increasing);



 /** 
 * @brief Receives "chunks" of sorted arrays, divided by direction. Sorts these in sequence, based on direction.
 */
void order_queue_sortChunksByDirection(struct Order* going_up, 
										struct Order* going_down, struct Order* other,
										int count_up, int count_down, int count_other, 
										int elevator_floor, HardwareMovement direction);

/**
 * @brief Sorts entire order queue, based on direction
 */
void order_queue_sortOrderQueue(int elevator_floor, HardwareMovement direction);

/**
 * @brief Adds a single, valid order
 *
 * @return 1 if true, 0 if false.
 */
int order_queue_add_order(struct Order* order, int elevator_floor, HardwareMovement direction);

/**
 * @brief Checks if ordering queue is empty.
*/
int order_queue_empty();


