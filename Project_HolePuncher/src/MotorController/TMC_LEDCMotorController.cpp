#include "MotorController/TMC_LEDCMotorController.h"
#include "PuncherConf.h"

const motor_feature_t TMC_LEDCMotorController::_feature = {1, 1, 1, 1, 0, 0};

TMC_LEDCMotorController::TMC_LEDCMotorController(int motor_steps, int micro_steps, int dir_pin, int step_pin, int enable_pin, int ledc_pcnt_channel, HardwareSerial * SerialPort, float RS, uint8_t addr)
{
    stepper = new LEDCStepperDriver(motor_steps, dir_pin, step_pin, enable_pin, ledc_pcnt_channel);
    driver = new TMC2209Stepper(SerialPort, RS, addr);

    this->dir_pin = dir_pin;
    this->step_pin = step_pin;
    this->enable_pin = enable_pin;
    this->motor_steps = motor_steps;
    this->micro_steps = micro_steps;
}

TMC_LEDCMotorController::~TMC_LEDCMotorController()
{
}

motor_res_t TMC_LEDCMotorController::begin()
{
    stepper->begin(189, micro_steps);   // 189 = 8mm * pi * 60s
    stepper->setEnableActiveState(LOW);

    if (!driver_serial->baudRate())
    {
        driver_serial->begin(DRIVER_UART_BANDRATE);
    }

    driver->begin();
    driver->microsteps(micro_steps);

    return MOTOR_RES_SUCCESS;
}

motor_res_t TMC_LEDCMotorController::move(long steps)
{
    stepper->move(steps);
    return MOTOR_RES_SUCCESS;
}

motor_res_t TMC_LEDCMotorController::rotate_infinite(int dir);
{
    stepper->rotate_infinite(int dir);
    return MOTOR_RES_SUCCESS;
}

long TMC_LEDCMotorController::stop()
{
    return stepper->stop();
}

long TMC_LEDCMotorController::get_position()
{
    return stepper->getPosition();
}

motor_res_t TMC_LEDCMotorController::setSpeed(long stepsPerSec)
{
    stepper->setPulseFreq(stepsPerSec);
    return MOTOR_RES_SUCCESS;
}

motor_status_t TMC_LEDCMotorController::getStatus()
{
    return stepper->isRunning() ? MOTOR_RUNNING : MOTOR_IDLE;
}

motor_res_t TMC_LEDCMotorController::setAutoSleep(bool autoSleep)
{   
    use_auto_sleep = autoSleep;
    stepper->setAutoSleep(autoSleep);
    return MOTOR_RES_SUCCESS;
}

motor_res_t TMC_LEDCMotorController::setActiveState(uint8_t activeState)
{
    stepper->setEnableActiveState(activeState);
    return MOTOR_RES_SUCCESS;
}

motor_res_t TMC_LEDCMotorController::calibrate()
{
    if (stepper->isRunning()) return MOTOR_RES_FAIL;
    stepper->rotate_infinite(this->calibration_direction);

    // TODO

    return MOTOR_RES_SUCCESS;
}

motor_res_t TMC_LEDCMotorController::setAutoCal(bool autoCal)
{
    use_calibration = autoCal;
    return MOTOR_RES_SUCCESS;
}

motor_res_t TMC_LEDCMotorController::setCalThres(uint8_t thres)
{   
    // TODO

    return MOTOR_RES_SUCCESS;
}

motor_res_t TMC_LEDCMotorController::setRunningCurrent(int rms_current)
{
    running_current = rms_current;
    return MOTOR_RES_SUCCESS;
}


std::vector<ConfigurableItem> TMC_LEDCMotorController::getConfigurations(bool full)
{
    std::vector<ConfigurableItem> itemList = MotorController::getConfigurations(full);

    itemList.push_back(ConfigurableInteger("Runing Current (mA)", ([this](void *p) { return this->setRunningCurrent(*(int *)p); }), running_current));
    itemList.push_back(ConfigurableBoolean("Auto sleep", ([this](void *p) { return this->setAutoSleep(*(int *)p); }), use_auto_sleep));
    itemList.push_back(ConfigurableBoolean("Auto calibrate", ([this](void *p) { return this->setAutoCal(*(int *)p); }), use_calibration));
    itemList.push_back(ConfigurableInteger("Calibrate Threshold", ([this](void *p) { return this->setCalThres(*(int *)p); }), calibration_threshold, 255, 0));
    itemList.push_back(ConfigurableCallback("Manual calibrate", ([this](void*) { return this->calibrate(); })));

    return itemList;
}
