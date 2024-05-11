#ifndef _TMC_LEDCMOTORCONTROLLER_H_
#define _TMC_LEDCMOTORCONTROLLER_H_

#include "MotorController.h"
#include "PuncherSemaphore.h"

#include <LEDCStepperDriver.h>
#include <TMCStepper.h>

class TMC_LEDCMotorController : public MotorController
{
private:
    LEDCStepperDriver *stepper;
    TMC2209Stepper *driver;

    uint8_t dir_pin;
    uint8_t step_pin;
    uint8_t enable_pin;
    uint8_t int_pin;
    int motor_steps;
    int micro_steps;
    HardwareSerial *driver_serial;

    uint8_t reverse = 0;

    /* feature settings */
    uint8_t use_auto_sleep : 1;

    /* basic values */
    uint8_t active_state;
    
    /* status for isr */
    bool is_zeroing;

    /* for zeroing */
    std::function<void()> zeroing_finish_callback = NULL;

    friend void evt_loop(void *param);
    friend void driver_diag_intr_handler(void *arg);

public:
    TMC_LEDCMotorController(int motor_steps, int micro_steps, int dir_pin, int step_pin, int enable_pin, int int_pin, int ledc_pcnt_channel, HardwareSerial *SerialPort, float RS, uint8_t addr);
    ~TMC_LEDCMotorController();

    /* Begin function */
    motor_res_t begin();
    /* Move steps */
    motor_res_t move(int32_t steps);
    /* Rotate in a direction */
    motor_res_t rotate_infinite(int dir);
    /* Stop and return steps remains */
    int32_t stop();
    /* Get motor status */
    motor_status_t getStatus();
    /* Get motor position (steps) */
    int32_t getPosition();
    /* Set motor in sleep state */
    motor_res_t sleep(bool sleep);
    /* Set motor sleep io active behavior */
    motor_res_t setActiveState(uint8_t activeState);
    /* Set motor to run in opposite direction */
    motor_res_t setReverse(bool reverse);

    /* Set motor speed */
    motor_res_t setSpeed(uint32_t s);
    /* Set motor current */
    motor_res_t setCurrent(uint32_t current);
    /* Set motor microstep */
    motor_res_t setMicroSteps(uint32_t microSteps);

    /* Check driver existance */
    motor_res_t pingDriver();
    /* Check motor existance */
    motor_res_t pingMotor();

    /* Load monitor */
    int32_t getLoad();

    /* Start Zeroing */
    motor_res_t startZeroing(int dir, int thres);

    /* Movement finish callback */
    motor_res_t setMoveFinishCallBack(std::function<void(void*)> cb, void* arg);    // From ISR
    /* Zeroing finish callback */
    motor_res_t setZeroingFinishCallBack(std::function<void()> cb); // NOT from ISR
};

#endif // _TMC_LEDCMOTORCONTROLLER_H_