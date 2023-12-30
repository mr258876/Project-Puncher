#ifndef _PUNCHERMOTORS_H_
#define _PUNCHERMOTORS_H_

#include "MotorController/MotorController.h"
#include "PositionSensor/PositionSensor.h"

extern MotorController *controller_X;
extern MotorController *controller_Y;
extern MotorController *controller_Z;

extern PositionSensor *sensor_X;
extern PositionSensor *sensor_Y;
extern PositionSensor *sensor_Z;

#endif