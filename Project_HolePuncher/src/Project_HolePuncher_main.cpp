#include <Arduino.h>
#include "PuncherConf.h"
#include "PuncherSemaphore.h"

#include <lvgl.h>

#include "PuncherScheduler.h"
#include "PuncherMotors.h"
// #include "SerialCommand.h"

#include "MotorController/TMC_LEDCMotorController.h"
#include "ui/LVGL_PuncherUI.h"

static PuncherScheduler *scheduler;



// ---------- Function declarations
void motor_init();
void lvgl_init();

void setup()
{
    ESP_LOGI("Puncher_Main", "Initializing...");

    pm_init();

    scheduler = new PuncherScheduler();
    scheduler->begin();

    motor_init();

    lvgl_init();

    ESP_LOGI("Puncher_Main", "System booted.");
}

void motor_init()
{
    ESP_LOGI("Puncher_Main", "Motor initializing...");

    DRIVER_SERIAL.begin(DRIVER_UART_BANDRATE, 134217756U, 44, 43);

    controller_X = new TMC_LEDCMotorController(MOTOR_STEPS, MICROSTEPS_X, xdirPin, xstepPin, xenablePin, X_CONTROL_CHANNEL, &DRIVER_SERIAL, R_SENSE, 0b00);
    controller_Y = new TMC_LEDCMotorController(MOTOR_STEPS, MICROSTEPS_Y, ydirPin, ystepPin, yenablePin, Y_CONTROL_CHANNEL, &DRIVER_SERIAL, R_SENSE, 0b01);
    controller_Z = new TMC_LEDCMotorController(MOTOR_STEPS, MICROSTEPS_Z, zdirPin, zstepPin, zenablePin, Z_CONTROL_CHANNEL, &DRIVER_SERIAL, R_SENSE, 0b10);

    scheduler->attachMotors(controller_X, controller_Y, controller_Z);

    controller_X->begin();
    controller_Y->begin();
    controller_Z->begin();

    controller_X->setActiveState(LOW);
    controller_Y->setActiveState(LOW);
    controller_Z->setActiveState(LOW);

    ESP_LOGI("Puncher_Main", "Motor initialized!");
}

void lvgl_init()
{
    ESP_LOGI("Puncher_Main", "Starting LVGL...");

    lvgl_ui->setLanguage(scheduler->getDisplayLanguage());
    lvgl_ui->begin();

    ESP_LOGI("Puncher_Main", "LVGL initialized!");

    ESP_LOGI("Puncher_Main", "Configuring LVGL UI");

    scheduler->attachUI(lvgl_ui);

    ESP_LOGI("Puncher_Main", "LVGL UI ready!");
}

void loop()
{
    scheduler->tick();
    if (xSemaphoreTake(LVGLMutex, portMAX_DELAY) == pdTRUE)
    {
        lv_timer_handler(); /* let the GUI do its work */
        xSemaphoreGive(LVGLMutex);
    }
    taskYIELD();
}