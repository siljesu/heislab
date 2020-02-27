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
//static RelativePosition* p_relative_position;
extern int g_FLOOR;
extern HardwareMovement g_currentMoveDirection;

typedef enum
{
    IDLE,
    MOVE,
    HANDLE_ORDER,
    DOORS_OPEN_TIMER,
    EMERGENCY_STOP
} State;

/**
* @brief state function pointer typedefed. 
* 
**/
typedef State (*StateFunction)();

/**
* @brief Initializes arrays and resets lights.
* 
**/
void elevator_init();

/**
* @brief Checks if someone places any valid order at any place. Will initialize this order and adds it to orderQueue.
*
* @param
**/
void elevator_checkAndAddOrder(int currentFloor, HardwareMovement moveDirection);

/**
* @brief Checks if target destination (first element of orderQueue) is reached.
*
* @param targetFloor Floor of first element in orderQueue
*
* @return 1 if true, 0 if not reached yet.
**/
int elevator_amIAtFloor(int targetFloor);

/**
* @brief Checks whether the elevator is at a floor or inbetween floors. Note: this function does not check which floor the elevator might be at.
*
* @return 0 if between floors, 1 if at a floor. Note that it coule be at any of the available floors if 1 is returned!
**/
int elevator_amIAtAnyFloor();

/**
* @brief Checks if new floor is reached.
*
* @param targetFloor The last floor you visited
*
* @return returns new floor if new signal is read, returns old floor if no new signals are read.
**/
int elevator_findCurrentFloor(int lastFloor);

/**
* @brief Sets new relative position when needed. Keeps track of where we are.
*
* @param moveDirection the elevators current move direction
**/
void elevator_setRelativePosition(HardwareMovement moveDirection);

void elevator_stopMotor();

void elevator_goDown();

void elevator_goUp();

void elevator_openDoors();

void elevator_closeDoors();

void elevator_handleOrder();
