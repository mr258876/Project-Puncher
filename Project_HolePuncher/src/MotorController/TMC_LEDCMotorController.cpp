#include "TMC_LEDCMotorController.h"

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
    stepper->begin(60, micro_steps);
    stepper->setEnableActiveState(LOW);

    if (!driver_serial->baudRate())
    {
        driver_serial->begin(115200);
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

/*
LEDCStepperDriver stepperX(MOTOR_STEPS, xdirPin, xstepPin, xenablePin, 0);
LEDCStepperDriver stepperY(MOTOR_STEPS, ydirPin, ystepPin, yenablePin, 1);
LEDCStepperDriver stepperZ(MOTOR_STEPS, zdirPin, zstepPin, zenablePin, 2);

TMC2209Stepper driverX(&DRIVER_SERIAL, R_SENSE, 0b00);
TMC2209Stepper driverY(&DRIVER_SERIAL, R_SENSE, 0b01);
TMC2209Stepper driverZ(&DRIVER_SERIAL, R_SENSE, 0b10);

void motor_init()
{
    // 初始化电机
    pinMode(xdiagPin, INPUT);
    pinMode(ydiagPin, INPUT);
    pinMode(zdiagPin, INPUT);

    getStepperPerimeters();

    stepperX.begin(menuRunningSpeedX.getAsFloatingPointValue() / xPerimeter * 60, MICROSTEPS_X);
    stepperX.setEnableActiveState(LOW);
    stepperY.begin(menuRunningSpeedY.getAsFloatingPointValue() / yPerimeter * 60, MICROSTEPS_Y);
    stepperY.setEnableActiveState(LOW);
    stepperY.setAutoSleep(true);
    stepperZ.begin(menuRunningSpeedZ.getAsFloatingPointValue() / zPerimeter * 60, MICROSTEPS_Z);
    stepperZ.setEnableActiveState(LOW);
    stepperX.enable();
    stepperY.enable();
    stepperZ.enable();

    DRIVER_SERIAL.begin(115200);                                // 启动串口
    driverX.begin();                                            // x方向驱动板开始通讯
    driverX.microsteps(MICROSTEPS_X);                           // x方向驱动板设置微步大小
    driverX.rms_current(menuRunningCurrentX.getCurrentValue()); // x方向驱动板设置电流大小 (mA)
    driverY.begin();                                            // y方向驱动板开始通讯
    driverY.microsteps(MICROSTEPS_Y);                           // y方向驱动板设置微步大小
    driverY.rms_current(menuRunningCurrentY.getCurrentValue()); // y方向驱动板设置电流大小 (mA)
    driverZ.begin();                                            // z方向驱动板开始通讯
    driverZ.microsteps(MICROSTEPS_Z);                           // z方向驱动板设置微步大小
    driverZ.rms_current(menuRunningCurrentZ.getCurrentValue()); // z方向驱动板设置电流大小 (mA)
}
*/