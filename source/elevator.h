/**
* @file
* @brief elevator operations and states are in this file.
**/

#include "orderQueue.h"

#define NUMBER_OF_FLOORS 4
#define NUMBER_OF_ORDER_TYPES 3


typedef enum {BELOW, ABOVE, AT} RelativePosition; //1 for above current floor, 0 for below current floor
extern RelativePosition relative_position;
//static RelativePosition* p_relative_position;

/**
* @brief state function pointer typedefed. 
* 
**/
typedef void (*state)();

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

//--------------states------------------//

// /**
// * @brief State: idle. Will stay idle at a defined floor until stop button is pressed or new order arrives, either up or down or to enter elevator. 
// *
// * @param
// *
// * @return void function
// **/
// void s_idle(int floor, HardwareMovement moveDirection);

// /**
// * @brief State: moving up. Will initialize hardware movement until target floor reached (if new order arrives which gets higher priority, target floor can change).
// *
// * @param
// **/
// void s_movingUp(int floor, HardwareMovement moveDirection);

// /**
// * @brief State: moving down. Will initialize hardware movement until target floor reached (if new order arrives which gets higher priority, target floor can change).
// *
// * @param
// *
// * @return void function
// **/
// void s_movingDown(int floor, HardwareMovement moveDirection);

// /**
// * @brief State: handle order. stops and opens door for three seconds. goes into idle state when finished.
// *
// * @param
// *
// * @return void function
// **/
// void s_handleOrder(int floor, HardwareMovement moveDirection);

// /**
// * @brief State: emergency stop. Will immidiately stop motors, and clear order queue. If at a floor, will open doors. Goes into idle or idleBetweenFloors.
// *
// * @param
// *
// * @return void function
// **/
// void s_emergencyStop(int floor, HardwareMovement moveDirection);

// /**
// * @brief State: obstruction. Something is blocking the door, will keep doors open until this is okay.
// *
// * @param
// *
// * @return void function
// **/
// void s_obstruction(int floor, HardwareMovement moveDirection);

// /**
// * @brief State: idle between floors. Behaves like idle, only in between floors.
// *
// * @param
// *
// * @return void function
// **/
// void s_idleInBetweenFloors(int floor, HardwareMovement moveDirection);
