/**
 * @file
 * @brief Door functionality
 *
 */

/**
 * @brief Initializes the door.
 *
 * @return 0 on success. Non-zero for failure.
 */
int door_init();

/**
 * @brief Forces doors open indefinitely, activates door_open_for_three_sec() upon completion.
 *
 * @warning will only happen under certain circumstances
 * @return 0 on success. Non-zero for failure.
 */
int door_stay_open(int stay_open);

/**
 * @brief Opens door for a duration of three seconds. Returns to closed door state after.
 *
 * @return 0 on success. Non-zero for failure.
 */
int door_open_for_three_sec();