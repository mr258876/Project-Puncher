#include "MotorController/TMC_LEDCMotorController.h"
#include "PuncherConf.h"

TMC_LEDCMotorController::TMC_LEDCMotorController(int motor_steps, int micro_steps, int dir_pin, int step_pin, int enable_pin, int ledc_pcnt_channel, HardwareSerial *SerialPort, float RS, uint8_t addr)
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

    xSemaphoreTake(*DUARTMutex, portMAX_DELAY);
    {
        driver->begin();
        driver->microsteps(micro_steps);
    }
    xSemaphoreGive(*DUARTMutex);

    return MOTOR_RES_SUCCESS;
}

motor_res_t TMC_LEDCMotorController::move(long steps)
{
    stepper->move(steps);
    return MOTOR_RES_SUCCESS;
}

motor_res_t TMC_LEDCMotorController::rotate_infinite(int dir)
{
    stepper->rotate_infinite(dir);
    return MOTOR_RES_SUCCESS;
}

long TMC_LEDCMotorController::stop()
{
    return stepper->stop();
}

long TMC_LEDCMotorController::getPosition()
{
    return stepper->getPosition();
}

motor_status_t TMC_LEDCMotorController::getStatus()
{
    return stepper->isRunning() ? MOTOR_RUNNING : MOTOR_IDLE;
}

motor_res_t TMC_LEDCMotorController::setActiveState(uint8_t activeState)
{
    stepper->setEnableActiveState(activeState);
    return MOTOR_RES_SUCCESS;
}

motor_res_t TMC_LEDCMotorController::sleep(bool sleep)
{
    if (sleep)
        stepper->disable();
    else
        stepper->enable();
    return MOTOR_RES_SUCCESS;
}

motor_res_t TMC_LEDCMotorController::setSpeed(uint32_t stepsPerSec)
{
    stepper->setPulseFreq(stepsPerSec);
    return MOTOR_RES_SUCCESS;
}

motor_res_t TMC_LEDCMotorController::setCurrent(uint32_t rms_current)
{
    xSemaphoreTake(*DUARTMutex, portMAX_DELAY);
    {
        driver->rms_current(rms_current);
    }
    xSemaphoreGive(*DUARTMutex);
    return MOTOR_RES_SUCCESS;
}

motor_res_t TMC_LEDCMotorController::pingDriver()
{
    uint8_t __state = digitalRead(enable_pin);

    stepper->disable();
    uint8_t en_state = 0;
    xSemaphoreTake(*DUARTMutex, portMAX_DELAY);
    {
        en_state = driver->IOIN() & 0x01;
    }
    xSemaphoreGive(*DUARTMutex);

    digitalWrite(enable_pin, __state); // write the original state back

    return en_state ? MOTOR_RES_SUCCESS : MOTOR_RES_FAIL;
}

motor_res_t TMC_LEDCMotorController::pingMotor()
{
    // running_current = rms_current;
    return MOTOR_RES_SUCCESS;
}