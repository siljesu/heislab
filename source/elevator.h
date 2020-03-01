/**
* @file
* @brief elevator operations and states are in this file.
**/

#include "orderQueue.h"

/**
 * @brief Global variable that is updated to always hold the elevator's last floor 
 * if between floors, or current floor if it's at a floor.
 */
extern int g_FLOOR;

/**
 * @brief Global variable that is updated to always hold the elevator's current movement direction. 
 * Note that stop is not considered a direction. In cases of the elevator standing still, 
 * the elevator's last direction is held.
 */
extern HardwareMovement g_currentMoveDirection;

/**
 * @brief Global pointer that points to the first element of orderQueue.
 */
Order* p_firstOrder;

/**
 * @brief Enum containing values to indicate elevator's current state. 
 * Used in the state machine implemented in @c main.c.
 */
typedef enum
{
    IDLE,
    MOVE,
    HANDLE_ORDER,
    DOORS_OPEN_TIMER,
    EMERGENCY_STOP
} State;

/**
* @brief State function pointer typedef-ed. Used in transitioning between states in @c main.c.
**/
typedef State (*StateFunction)();

/**
* @brief Initializes arrays and resets lights.
**/
void elevator_init();

/**
* @brief Checks if new floor is reached.
*
* @param lastFloor The last floor you visited.
*
* @return New floor if new signal is read, returns @p lastFloor floor if no new signals are read.
**/
int elevator_findCurrentFloor(int lastFloor);

/**
* @brief Checks if target destination (first element of orderQueue) is reached.
*
* @param targetFloor Floor of first element in orderQueue.
*
* @return 1 if true, 0 if not yet reached.
**/
int elevator_amIAtFloor(int targetFloor);

/**
* @brief Checks whether the elevator is at a floor or inbetween floors. 
* Note that this function does not check which floor the elevator might be at, only whether it's at one or not.
*
* @return 0 if between floors, 1 if at a floor. 
* Note that it could be at any of the available floors if 1 is returned!
**/
int elevator_amIAtAnyFloor();

/**
* @brief Checks if someone places any valid order at any place. 
* Will initialize this order and adds it to orderQueue.
*
* @param currentFloor The elevator's current floor, upon receiving a request to add an order.
* 
* @param moveDirection The elevator's current direction, upon receiving a request to add an order.
**/
void elevator_checkAndAddOrder(int currentFloor, HardwareMovement moveDirection);

/**
* @brief Sets new @c g_relativePosition when needed. Keeps track of where we are.
*
* @param moveDirection the elevators current move direction.
**/
void elevator_setRelativePosition(HardwareMovement moveDirection);

/**
* @brief Stops the motor.
**/
void elevator_stopMotor();

/**
* @brief Starts the motor, making the elevator go downwards.
**/
void elevator_goDown();

/**
* @brief Starts the motor, making the elevator go upwards. 
**/
void elevator_goUp();

/**
* @brief Turns on the open doors-light (opening the doors).
**/
void elevator_openDoors();

/**
* @brief Turns off the open doors-light (closing the doors). 
**/
void elevator_closeDoors();

/**
* @brief Removes all orders at the elevator's current floor from the orderQueue.
**/
void elevator_handleOrder();

/**
* @brief Checks whether there are any orders at the elevator's current floor or not.
*
* @return 1 if there are any orders at the elevator's current floor, 0 if not.
**/
int elevator_ordersAtThisFloor();

/**
* @brief Checks whether there is an obstruction or not.
*
* @return 1 if there is an obstruction, 0 if not.
**/
int elevator_obstruction();

/**
* @brief Checks if the stop-button is pressed.
*
* @return 1 if the stop-button is pressed, 0 if not.
**/
int elevator_stopSignal();

/**
* @brief Turns off the stop-light.
**/
void elevator_stopLightOff();

/**
* @brief Turns on the stop-light.
**/
void elevator_stopLightOn();

/**
* @brief Checks the criteria for going into state @c EMERGENCY_STOP.
*
* @return 1 if criteria are met, 0 if not.
**/
int elevator_checkForStop();

/**
* @brief Turns all orders' lights off.
*/
void elevator_clearAllOrderLights();

/**
* @brief Deletes all active orders in orderQueue.
*/
void elevator_clearAllOrders();