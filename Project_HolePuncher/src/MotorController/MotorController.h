#ifndef _MOTOR_CONTROL_H_
#define _MOTOR_CONTROL_H_

#include <stddef.h>
#include "Observe.h"
#include "Configurable.h"

enum motor_status_t
{
    MOTOR_ERR = -1,
    MOTOR_IDLE = 0,
    MOTOR_RUNNING,
};

enum motor_res_t
{
    MOTOR_RES_NOT_IMPLEMENTED = -1,
    MOTOR_RES_SUCCESS = 0,
    MOTOR_RES_FAIL,
};

struct motor_feature_t
{
    uint8_t setSpeed : 1;
    uint8_t calibrate : 1;
    uint8_t autoSleep : 1;
    uint8_t setCurrent : 1;
    uint8_t closedLoop : 1;
    uint8_t closedLoopPID : 1;
};

class MotorController : public Observeable, Configurable
{
private:
    /* feature settings */
    uint8_t length_is_circumference : 1;

    // uint8_t use_closed_loop : 1;
    /* basic values */
    float _length;
    float _speed;

public:
    /* Begin function */
    virtual motor_res_t begin() = 0;
    /* Move steps */
    virtual motor_res_t move(long steps) = 0;
    /* Rotate in a direction */
    virtual motor_res_t rotate_infinite(int dir) = 0;
    /* Stop and return steps remains */
    virtual long stop() = 0;
    /* Get motor status */
    virtual motor_status_t getStatus() = 0;
    /* Get motor position (steps) */
    virtual long getPosition() = 0;
    // virtual motor_feature_t getFeature() = 0;

    /*  Start calibration process.
        Scheduler will monitor motor status, so return an idle state after calibration process.
        Remeber to update motor position.
     */
    virtual motor_res_t calibrate() { return MOTOR_RES_NOT_IMPLEMENTED; };

    inline motor_res_t setLength(float l) { _length = l; return MOTOR_RES_SUCCESS; };
    inline motor_res_t setLengthCircumference(int is_circumference) { length_is_circumference = is_circumference; return MOTOR_RES_SUCCESS; }
    inline motor_res_t setSpeed(float s) { _speed = s; return MOTOR_RES_SUCCESS; };

    inline float getLength() { return _length; };
    inline float getSpeed() { return _speed; };

    inline std::vector<ConfigurableItem> getConfigurations(bool full=false)
    {
        std::vector<ConfigurableItem> itemList;

        itemList.push_back(ConfigurableFloat("Length", ([this](void *p) { return this->setLength(*(float*)p); }), _length));
        itemList.push_back(ConfigurableBoolean("Length is circumference", ([this](void *p) { return this->setLengthCircumference(*(int *)p); }), length_is_circumference, CONFIGURABLE_BOOLEAN_CHECKBOX));
        itemList.push_back(ConfigurableFloat("Speed", ([this](void *p) { return this->setSpeed(*(float*)p); }), _speed));

        return itemList;
    };
};

#endif // _MOTORCONTROL_H_