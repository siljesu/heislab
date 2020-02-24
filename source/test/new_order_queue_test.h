/**
 * @file
 * @brief Order queue functionality
 *
 */

#include <stdbool.h> //Exists in order.h. Remove once merged properly
#include "order_test.h"

/**
 * @brief Lookup table that shows "true" (1) for each order that's placed. Orders correspond with hardware.c, lines 127 - 132
 */
bool order_table[12];

Order testOrder;
Order* p_testOrder = &testOrder;
Order testOrder2;
Order* p_testOrder2 = &testOrder2;
Order testOrder3;
Order* p_testOrder3 = &testOrder3;
Order testOrder4;
Order* p_testOrder4 = &testOrder4;
Order testOrder5;
Order* p_testOrder5 = &testOrder5;


Order emptyOrder = {0,HARDWARE_MOVEMENT_UP,true};

Order going_up[12]; //Set to QUEUE_SIZE for redundancy, rather than limitation
Order going_down[12];
Order second_going_up[12]; // happens only if already going up
Order second_going_down[12]; // happens only if already going down




/**
 * @brief Table to hold up to 12 orders (12 is the max possible), sorted after proximity in the elevator's current path.
 */
Order order_queue[12];

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
void order_queue_shift();

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
