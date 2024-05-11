#ifndef _POSITIONSENSOR_H_
#define _POSITIONSENSOR_H_

#include "MotorController/MotorController.h"
#include <functional>

enum position_res_t
{
    POSITION_RES_NOT_IMPLEMENTED = -1,
    POSITION_RES_SUCCESS = 0,
    POSITION_RES_FAIL,
};

class PositionSensor
{
public:
    virtual void begin() = 0;
    virtual position_res_t ping() = 0;
    /* in RAD */
    virtual double getAngle() = 0;
    /* unsigned */
    virtual int16_t rawReading() = 0;
    /* set current position to zero */
    virtual void clearRelativePosition() = 0;
    /* degree since last setZero() */
    virtual double getRelativePosition() = 0;
    /* load calibration profile */
    virtual position_res_t loadCalibrationProfile(void *) = 0;
    // virtual void *getCalibrationProfile() = 0;
    virtual size_t getCalibrationProfileSize() = 0;

    virtual position_res_t startCalibration(MotorController *axis, uint32_t microsteps_per_revolution) = 0;
    virtual position_res_t setCalibrationFinishCallBack(std::function<void()> cb) = 0;
};

#endif