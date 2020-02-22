#include "hardware.h"

void s_idle(int floor, HardwareMovement moveDirection);

void s_movingDown(int floor, HardwareMovement moveDirection);

void s_movingUp(int floor, HardwareMovement moveDirection);

void s_handleOrder(int floor, HardwareMovement moveDirection);

void s_emergencyStop(int floor, HardwareMovement moveDirection);

void s_obstruction(int floor, HardwareMovement moveDirection);

void s_idleInBetweenFloors(int floor, HardwareMovement moveDirection);