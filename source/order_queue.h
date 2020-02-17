/**
 * @file
 * @brief Order queue functionality
 *
 */

#include "order.h"
#include <stdbool.h> //Exists in order.h. Remove once merged properly
#define QUEUE_SIZE

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
 * @brief Adds a single, valid order
 *
 * @param order_queue gives the currently first element in queue, for pointer location reference
 *
 * @return 0 on success. Non-zero for failure.
 */
int order_queue_add_order(struct Order* order, int elevator_floor, HardwareMovement direction);

/**
 * @brief Sorts queue, must be used when adding order.
 *
 * @return 0 on success. Non-zero for failure.
 */
void order_queue_sort_incrementally(struct Order* temp_array, bool increasing);

/**
 * @brief Calls a queue sort for every movement direction change.
 *
 */
void order_queue_sort_by_direction_change(HardwareMovement direction);

/*
 * @brief Sorts entire order queue, based on direction
 */
void order_queue_sortOrderQueue(struct Order* order, struct Order* order_queue, 
							int elevator_floor, HardwareMovement direction);
