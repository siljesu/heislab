/**
 * @file
 * @brief Order queue functionality
 *
 */

#include "order.h"

bool order_table[12];
Order order_queue[12];

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
 * @return 0 on success. Non-zero for failure.
 */
int order_queue_add_order(Order order);

/**
 * @brief Sorts queue, must be used when adding order.
 *
 * @return 0 on success. Non-zero for failure.
 */
int order_queue_sort();

/**
 * @brief Removes a handled order
 *
 * @return 0 on success. Non-zero for failure.
 */
int order_queue_remove_order(Order order);

/**
 * @brief Handles first order in queue.
 *
 * @return 0 on success. Non-zero for failure.
 */
int order_queue_handle_order(/*first element in queue*/);