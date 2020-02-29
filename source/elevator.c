#include <stdio.h>
#include <stdlib.h>
#include "elevator.h"

#define QUEUE_SIZE 12
RelativePosition g_relativePosition;
int g_FLOOR;
HardwareMovement g_currentMoveDirection;

void elevator_init()
{
    g_relativePosition = ABOVE; //because elevator always starts driving down
    hardware_command_clear_all_order_lights();

    int local_queue_size = 12;
    for (int i = 0; i < (local_queue_size); i++)
    {
        orderQueue[i] = EMPTYORDER;
        goingUp[i] = EMPTYORDER;
        goingDown[i] = EMPTYORDER;
        secondGoingUp[i] = EMPTYORDER;
        secondGoingDown[i] = EMPTYORDER;
    }

    for (int i = 0; i < NUMBER_OF_FLOORS; i++)
    {
        for (int j = 0; j < NUMBER_OF_ORDER_TYPES; j++)
        {
            orderTable[i][j] = 0;
        }
    }

    elevator_goDown();
}

int elevator_findCurrentFloor(int lastFloor)
{
    for (int f = 0; f < NUMBER_OF_FLOORS; f++)
    {
        if (hardware_read_floor_sensor(f))
        {
            g_FLOOR = f;
            hardware_command_floor_indicator_on(f);
            return f;
        }
    }
    g_FLOOR = lastFloor;
    return lastFloor;
}

int elevator_amIAtFloor(int targetFloor)
{
    for (int i = 0; i < NUMBER_OF_FLOORS; i++)
    {
        if (hardware_read_floor_sensor(i) && (i == targetFloor))
        {
            g_FLOOR = i;
            hardware_command_floor_indicator_on(i);
            return 1;
        }
    }
    return 0;
}

int elevator_amIAtAnyFloor()
{
    for (int i = 0; i < NUMBER_OF_FLOORS; i++)
    {
        if (hardware_read_floor_sensor(i))
        {
            g_FLOOR = i;
            hardware_command_floor_indicator_on(i);
            return 1;
        }
    }
    return 0;
}

void elevator_checkAndAddOrder(int currentFloor, HardwareMovement moveDirection)
{
    for (int i = 0; i < NUMBER_OF_FLOORS; i++)
    {
        if (hardware_read_order(i, HARDWARE_ORDER_UP))
        {
            Order order = order_init(i,HARDWARE_ORDER_UP);
            if (order_checkUnique(order))
            {
                orderQueue_addOrder(order, currentFloor, moveDirection);
                order_toggleUnique(order,1);
            }
        }
        if (hardware_read_order(i, HARDWARE_ORDER_DOWN))
        {
            Order order = order_init(i,HARDWARE_ORDER_DOWN);
            if (order_checkUnique(order))
            {
                orderQueue_addOrder(order, currentFloor, moveDirection);    
                order_toggleUnique(order,1);
            }
        }
        if (hardware_read_order(i, HARDWARE_ORDER_INSIDE))
        {
            Order order = order_init(i,HARDWARE_ORDER_INSIDE);
            if (order_checkUnique(order))
            {
                orderQueue_addOrder(order, currentFloor, moveDirection);    
                order_toggleUnique(order,1);
            }
        }
    }
}

void elevator_setRelativePosition(HardwareMovement moveDirection)
{
    if (moveDirection == HARDWARE_MOVEMENT_UP)
    {
        if (g_relativePosition == AT && (!elevator_amIAtAnyFloor()))
        {
            g_relativePosition = ABOVE;
        }
        else if (g_relativePosition != AT && (elevator_amIAtAnyFloor()))
        {
            g_relativePosition = AT;
        }
    }
    if (moveDirection == HARDWARE_MOVEMENT_DOWN)
    {
        if (g_relativePosition == AT && (!elevator_amIAtAnyFloor()))
        {
            g_relativePosition = BELOW;
        }
        else if (g_relativePosition != AT && (elevator_amIAtAnyFloor()))
        {
            g_relativePosition = AT;
        }
    }
}

void elevator_stopMotor()
{
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
}
void elevator_goDown()
{
    hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    g_currentMoveDirection = HARDWARE_MOVEMENT_DOWN;
}

void elevator_goUp()
{
    hardware_command_movement(HARDWARE_MOVEMENT_UP);
    g_currentMoveDirection = HARDWARE_MOVEMENT_UP;
}

void elevator_openDoors()
{
    hardware_command_door_open(1);
}

void elevator_closeDoors()
{
    hardware_command_door_open(0);
}

void elevator_handleOrder()
{
    for (int i = (QUEUE_SIZE); i > -1; i--)
    { //hardcoded queuesize
        if ((orderQueue[i].floor == g_FLOOR) && orderQueue[i].activeOrder)
        {
            orderQueue_deleteByShiftingAtIndex(i);
        }
    }
}

int elevator_ordersAtThisFloor() {
    for (int i = (QUEUE_SIZE); i > -1; i--){
        if ((orderQueue[i].floor == g_FLOOR) && orderQueue[i].activeOrder)
        {
            return 1;
        }
    }
    return 0;
}

int elevator_obstruction() {
    return hardware_read_obstruction_signal();
}

int elevator_stopSignal() {
    return hardware_read_stop_signal();
}

void elevator_stopLightOff(){
    hardware_command_stop_light(0);
}

void elevator_stopLightOn(){
    hardware_command_stop_light(0);
}

int elevator_checkForStop(){
    if (elevator_stopSignal() && elevator_amIAtAnyFloor())
    {
        elevator_stopMotor();
        elevator_openDoors();
        return 1;
        //return EMERGENCY_STOP;
    }
    if (elevator_stopSignal() && !elevator_amIAtAnyFloor())
    {
        elevator_stopMotor();
        return 1;
        //return EMERGENCY_STOP;
    }
    return 0;
}