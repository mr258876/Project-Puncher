#include "MotorController/TMC_LEDCMotorController.h"
#include "PuncherConf.h"
#include <hal/gpio_hal.h>

static TaskHandle_t evt_loop_handler = NULL;
static QueueHandle_t evt_queue = NULL;

typedef enum
{
    MC_EVT_NONE = 0,
    MC_EVT_ZEROING_DONE,
} TMC_LEDCMotorController_evt_t;

struct TMC_LEDCMotorController_evt_msg
{
    TMC_LEDCMotorController *controller;
    TMC_LEDCMotorController_evt_t msg;
};

void evt_loop(void *param)
{
    QueueHandle_t queue = *(QueueHandle_t *)param;
    TMC_LEDCMotorController_evt_msg msg;

    while (1)
    {
        xQueueReceive(queue, &msg, portMAX_DELAY);
        switch (msg.msg)
        {
        case MC_EVT_ZEROING_DONE:
            xSemaphoreTake(*DUARTMutex, portMAX_DELAY);
            {
                msg.controller->driver->TCOOLTHRS(0);
            }
            xSemaphoreGive(*DUARTMutex);

            if (msg.controller->zeroing_finish_callback)
            {
                msg.controller->zeroing_finish_callback();
            }
            break;

        default:
            break;
        }
    }

    vTaskDelete(NULL);
}

void IRAM_ATTR driver_diag_intr_handler(void *arg)
{
    TMC_LEDCMotorController *controller = (TMC_LEDCMotorController *)arg;

    if (controller->is_zeroing)
    {
        controller->stepper->__stop_from_int();
        TMC_LEDCMotorController_evt_msg msg = {controller, MC_EVT_ZEROING_DONE};
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xQueueSendFromISR(evt_queue, &msg, &xHigherPriorityTaskWoken);
    }
}

TMC_LEDCMotorController::TMC_LEDCMotorController(int motor_steps, int micro_steps, int dir_pin, int step_pin, int enable_pin, int int_pin, int ledc_pcnt_channel, HardwareSerial *SerialPort, float RS, uint8_t addr)
{
    stepper = new LEDCStepperDriver(motor_steps, dir_pin, step_pin, enable_pin, ledc_pcnt_channel);
    driver = new TMC2209Stepper(SerialPort, RS, addr);

    this->dir_pin = dir_pin;
    this->step_pin = step_pin;
    this->enable_pin = enable_pin;
    this->int_pin = int_pin;
    this->motor_steps = motor_steps;
    this->micro_steps = micro_steps;
}

TMC_LEDCMotorController::~TMC_LEDCMotorController()
{
}

motor_res_t TMC_LEDCMotorController::begin()
{
    /* Create queue and evt loop to process events */
    if (!evt_loop_handler)
    {
        evt_queue = xQueueCreate(8, sizeof(TMC_LEDCMotorController_evt_msg));
        xTaskCreate(evt_loop, "TLMCevtLoop", 4096, &evt_queue, 1, &evt_loop_handler);
    }

    /*  Notice:
        init process must follows:
            stepper init -> ISR init -> UART driver init
        or LEDC in stepper will not work.
        Nobody knows why :(
     */

    stepper->begin(60, micro_steps);
    stepper->setEnableActiveState(LOW);

    if (int_pin != 255)
    {
        pinMode(int_pin, INPUT_PULLDOWN);
        attachInterruptArg(digitalPinToInterrupt(int_pin), driver_diag_intr_handler, this, RISING);
    }

    xSemaphoreTake(*DUARTMutex, portMAX_DELAY);
    {
        driver->begin();
        driver->microsteps(micro_steps);
        driver->TCOOLTHRS(0);
        driver->dedge(0);   // Make sure we only step on rising edge
    }
    xSemaphoreGive(*DUARTMutex);

    return MOTOR_RES_SUCCESS;
}

motor_res_t TMC_LEDCMotorController::move(long steps)
{
    stepper->startMove(reverse ? -steps : steps);
    return MOTOR_RES_SUCCESS;
}

motor_res_t TMC_LEDCMotorController::rotate_infinite(int dir)
{
    stepper->rotate_infinite(reverse ? -dir : dir);
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

motor_res_t TMC_LEDCMotorController::setReverse(bool reverse)
{
    this->reverse = reverse;
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

motor_res_t TMC_LEDCMotorController::setMicroSteps(uint32_t microSteps)
{
    xSemaphoreTake(*DUARTMutex, portMAX_DELAY);
    {
        driver->mstep_reg_select(1);
        // The function in TMCStepper is not working, wtf
        switch (microSteps)
        {
        case 256:
            driver->mres(0);
            break;
        case 128:
            driver->mres(1);
            break;
        case 64:
            driver->mres(2);
            break;
        case 32:
            driver->mres(3);
            break;
        case 16:
            driver->mres(4);
            break;
        case 8:
            driver->mres(5);
            break;
        case 4:
            driver->mres(6);
            break;
        case 2:
            driver->mres(7);
            break;
        case 0:
            driver->mres(8);
            break;
        default:
            break;
        }
    }
    xSemaphoreGive(*DUARTMutex);
    return MOTOR_RES_SUCCESS;
}

motor_res_t TMC_LEDCMotorController::pingDriver()
{
    uint8_t __state = digitalRead(enable_pin);

    digitalWrite(enable_pin, HIGH);
    uint32_t en_state = 0;
    xSemaphoreTake(*DUARTMutex, portMAX_DELAY);
    {
        en_state = driver->enn();
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

int TMC_LEDCMotorController::getLoad()
{
    int res = -1;
    xSemaphoreTake(*DUARTMutex, portMAX_DELAY);
    {
        res = driver->SG_RESULT();
    }
    xSemaphoreGive(*DUARTMutex);
    return res;
}

motor_res_t TMC_LEDCMotorController::startZeroing(int dir, int thres)
{
    xSemaphoreTake(*DUARTMutex, portMAX_DELAY);
    {
        // TCOOLTHRS needs to be set for stallgaurd to work //
        driver->TCOOLTHRS(0xFFFFF); // 20bit max
        driver->SGTHRS(thres / 2);  // stall when SG_RESULT ≤ SGTHRS*2
    }
    xSemaphoreGive(*DUARTMutex);
    this->is_zeroing = true;
    stepper->rotate_infinite(dir);
    return MOTOR_RES_SUCCESS;
}

motor_res_t TMC_LEDCMotorController::setMoveFinishCallBack(std::function<void(void*)> cb, void* arg)
{
    stepper->setFinishCallBack(cb, arg);
    return MOTOR_RES_SUCCESS;
}


motor_res_t TMC_LEDCMotorController::setZeroingFinishCallBack(std::function<void()> cb)
{
    this->zeroing_finish_callback = cb;
    return MOTOR_RES_SUCCESS;
}