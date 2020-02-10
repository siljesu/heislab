/**
 * @file
 * @brief Order queue functionality
 *
 */

#include "order.h"

bool order_table[12];
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
 * @return 0 on success. Non-zero for failure.
 */
int order_queue_add_order(struct Order order);

/**
 * @brief Sorts queue, must be used when adding order.
 *
 * @return 0 on success. Non-zero for failure.
 */
int order_queue_sort();


/**
 * @brief Checks if ordering queue is empty.
 *
 * @return 1 if true, 0 if false.
 */
int order_queue_empty();