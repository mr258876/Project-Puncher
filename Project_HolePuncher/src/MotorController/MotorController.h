#ifndef _MOTOR_CONTROL_H_
#define _MOTOR_CONTROL_H_

#include <stdint.h>
#include <stddef.h>

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

class MotorController
{
private:
    /* basic values */
    uint32_t _speed; // in pulse

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
    /* Set motor in sleep state */
    virtual motor_res_t sleep(bool sleep) = 0;
    /* Set motor sleep io active behavior */
    virtual motor_res_t setActiveState(uint8_t activeState) = 0;

    /* Set motor speed */
    virtual motor_res_t setSpeed(uint32_t s) = 0;
    /* Set motor current */
    virtual motor_res_t setCurrent(uint32_t current) = 0;

    /* Check driver existance */
    virtual motor_res_t pingDriver() = 0;
    /* Check motor existance */
    virtual motor_res_t pingMotor() = 0;
};

#endif // _MOTORCONTROL_H_