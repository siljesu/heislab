/**
* @file
* @brief elevator operations and states are in this file.
**/

#include "orderQueue.h"

#define NUMBER_OF_FLOORS 4
#define NUMBER_OF_ORDER_TYPES 3

typedef enum
{
    BELOW,
    ABOVE,
    AT
} RelativePosition; //1 for above current floor, 0 for below current floor

extern RelativePosition g_relativePosition;
extern int g_FLOOR;
extern HardwareMovement g_currentMoveDirection;

/**
 * @brief Enum containing values to indicate elevator's current state. Used in the state machine implemented in main.c
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
* @brief state function pointer typedefed. To be used in transitioning between states in main.c
**/
typedef State (*StateFunction)();

/**
* @brief Initializes arrays and resets lights.
**/
void elevator_init();

/**
* @brief Checks if new floor is reached.
*
* @param lastFloor The last floor you visited
*
* @return new floor if new signal is read, returns old floor if no new signals are read.
**/
int elevator_findCurrentFloor(int lastFloor);

/**
* @brief Checks if target destination (first element of orderQueue) is reached.
*
* @param targetFloor Floor of first element in orderQueue
*
* @return 1 if true, 0 if not yet reached.
**/
int elevator_amIAtFloor(int targetFloor);

/**
* @brief Checks whether the elevator is at a floor or inbetween floors. Note: this function does not check which floor the elevator might be at, only whether it's at one or not.
*
* @return 0 if between floors, 1 if at a floor. Note that it could be at any of the available floors if 1 is returned!
**/
int elevator_amIAtAnyFloor();

/**
* @brief Checks if someone places any valid order at any place. Will initialize this order and adds it to orderQueue.
*
* @param currentFloor The elevator's current floor, upon receiving a request to add an order.
* 
* @param moveDirection The elevator's current direction, upon receiving a request to add an order.
**/
void elevator_checkAndAddOrder(int currentFloor, HardwareMovement moveDirection);

/**
* @brief Sets new relative position when needed. Keeps track of where we are.
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
* @brief Disables the stop-light.
**/
void elevator_stopLightOff();

/**
* @brief Enables the stop-light.
**/
void elevator_stopLightOn();

/**
* @brief Checks the criteria for going into state EMERGENCY_STOP.
*
* @return 1 if criteria are met, 0 if not.
**/
int elevator_checkForStop();