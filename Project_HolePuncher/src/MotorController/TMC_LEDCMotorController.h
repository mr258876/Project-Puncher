#ifndef _TMC_LEDCMOTORCONTROLLER_H_
#define _TMC_LEDCMOTORCONTROLLER_H_

#include "MotorController.h"

#include <LEDCStepperDriver.h>
#include <TMCStepper.h>

class TMC_LEDCMotorController : public MotorController
{
private:
    static const motor_feature_t _feature;

    LEDCStepperDriver *stepper;
    TMC2209Stepper *driver;

    uint8_t dir_pin;
    uint8_t step_pin;
    uint8_t enable_pin;
    uint8_t int_pin;
    int motor_steps;
    int micro_steps;
    HardwareSerial *driver_serial;

    /* feature settings */
    uint8_t use_auto_sleep : 1;
    // uint8_t use_current_adj : 1;
    uint8_t use_calibration : 1;

    /* basic values */
    uint8_t active_state;
    int running_current;
    int sleep_current;
    int calibration_current;
    int calibration_threshold;
    int calibration_direction;
    // double closed_loop_calibration_value;

public:
    TMC_LEDCMotorController(int motor_steps, int micro_steps, int dir_pin, int step_pin, int enable_pin, int ledc_pcnt_channel, HardwareSerial *SerialPort, float RS, uint8_t addr);
    ~TMC_LEDCMotorController();

    inline motor_feature_t getFeature() { return _feature; };
    motor_res_t begin();
    motor_res_t move(long steps);
    motor_res_t setSpeed(long stepsPerSec);
    motor_status_t getStatus();
    /*  Start calibration process.
        Scheduler will monitor motor status, so return an idle state after calibration process.
        Remeber to update motor position.
     */
    motor_res_t calibrate();
    motor_res_t setAutoCal(bool autoCal);
    motor_res_t setCalThres(uint8_t thres);

    motor_res_t setAutoSleep(bool autoSleep);
    motor_res_t setActiveState(uint8_t activeState);

    motor_res_t setRunningCurrent(int rms_current);

    /* For user interface */
    std::vector<ConfigurableItem> getConfigurations(bool full=false);
};

#endif // _TMC_LEDCMOTORCONTROLLER_H_