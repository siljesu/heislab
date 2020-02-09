/**
 * @file
 * @brief Order queue functionality
 *
 */


struct Order {

    int floor;
    int floor_light_indicator_init;
    int valid;

    /**
     * @brief Initializes valid order.
     *
     * @return 0 on success. Non-zero for failure.
     */
    int order_init();

    /**
     * @brief Deletes order.
     *
     * @return 0 on success. Non-zero for failure.
     */
    int order_delete();

};
