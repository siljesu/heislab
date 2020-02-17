/**
* @file
* @brief elevator operations and states are in this file.
**/

#include "hardware.h"
#include "order.h"
#include "order_queue.h"

#define NUMBER_OF_FLOORS 4


/**
* @brief state function pointer typedefed. 
* 
**/
typedef void (*state)(int,HardwareMovement;


/**
* @brief Checks if someone places any valid order at any place. Will initialize this order and adds it to order_queue.
*
* @param
*
* @return void function
**/
void elevator_checkAndAddOrder(int currentFloor, HardwareMovement moveDirection)

/**
* @brief Checks if target destination (first element of order_queue) is reached.
*
* @param targetFloor Floor of first element in order_queue
*
* @return 1 if true, 0 if not reached yet.
**/
int elevator_amIAtFloor(int targetFloor)





//--------------states------------------//

/**
* @brief State: idle. Will stay idle at a defined floor until stop button is pressed or new order arrives, either up or down or to enter elevator. 
*
* @param
*
* @return void function
**/
void s_idle(int floor, HardwareMovement moveDirection)

/**
* @brief State: moving up. Will initialize hardware movement until target floor reached (if new order arrives which gets higher priority, target floor can change).
*
* @param
*
* @return void function
**/
void s_movingUp(int floor, HardwareMovement moveDirection)

/**
* @brief State: moving down. Will initialize hardware movement until target floor reached (if new order arrives which gets higher priority, target floor can change).
*
* @param
*
* @return void function
**/
void s_movingDown(int floor, HardwareMovement moveDirection)

/**
* @brief State: handle order. stops and opens door for three seconds. goes into idle state when finished.
*
* @param
*
* @return void function
**/
void s_handleOrder(int floor, HardwareMovement moveDirection)

/**
* @brief State: emergency stop. Will immidiately stop motors, and clear order queue. If at a floor, will open doors. Goes into idle or idleBetweenFloors.
*
* @param
*
* @return void function
**/
void s_emergencyStop(int floor, HardwareMovement moveDirection)

/**
* @brief State: obstruction. Something is blocking the door, will keep doors open until this is okay.
*
* @param
*
* @return void function
**/
void s_obstruction(int floor, HardwareMovement moveDirection)

/**
* @brief State: idle between floors. Behaves like idle, only in between floors.
*
* @param
*
* @return void function
**/
void s_idleInBetweenFloors(int floor, HardwareMovement moveDirection)