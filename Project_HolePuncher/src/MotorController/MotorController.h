#ifndef _MOTOR_CONTROL_H_
#define _MOTOR_CONTROL_H_

#include <stdint.h>
#include <stddef.h>
#include <functional>

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
    virtual motor_res_t move(int32_t steps) = 0;
    /* Rotate in a direction */
    virtual motor_res_t rotate_infinite(int dir) = 0;
    /* Stop and return steps remains */
    virtual int32_t stop() = 0;
    /* Get motor status */
    virtual motor_status_t getStatus() = 0;
    /* Get motor position (steps) */
    virtual int32_t getPosition() = 0;
    /* Set motor in sleep state */
    virtual motor_res_t sleep(bool sleep) = 0;
    /* Set motor sleep io active behavior */
    virtual motor_res_t setActiveState(uint8_t activeState) = 0;
    /* Set motor to run in opposite direction */
    virtual motor_res_t setReverse(bool reverse) = 0;

    /* Set motor speed */
    virtual motor_res_t setSpeed(uint32_t s) = 0;
    /* Set motor current */
    virtual motor_res_t setCurrent(uint32_t current) = 0;
    /* Set motor microstep */
    virtual motor_res_t setMicroSteps(uint32_t microSteps) = 0;

    /* Check driver existance */
    virtual motor_res_t pingDriver() = 0;
    /* Check motor existance */
    virtual motor_res_t pingMotor() = 0;

    /* Load monitor */
    virtual int32_t getLoad() = 0;
    
    /* Start Zeroing */
    virtual motor_res_t startZeroing(int dir, int thres) = 0;

    /* Movement finish callback, will be called in isr */
    virtual motor_res_t setMoveFinishCallBack(std::function<void(void*)> cb, void* arg) = 0;
    /* Zeroing finish callback */
    virtual motor_res_t setZeroingFinishCallBack(std::function<void()> cb) = 0;
};

#endif // _MOTORCONTROL_H_