#include <Arduino.h>
#include "PuncherConf.h"
#include "PuncherSemaphore.h"

#include <lvgl.h>

#include "PuncherScheduler.h"
#include "PuncherMotors.h"

#include "MotorController/TMC_LEDCMotorController.h"
#include "ui/LVGL_PuncherUI.h"
#include "SerialInterface/SerialInterface.h"

static PuncherScheduler *scheduler;



// ---------- Function declarations
void motor_init();
void lvgl_init();
void serial_init();

void setup()
{
    ESP_LOGI("Puncher_Main", "Initializing...");

    pm_init();  // Init power manager first or voltage acquire will fail

    scheduler = new PuncherScheduler();
    scheduler->begin();

    motor_init();

    lvgl_init();

    serial_init();

    ESP_LOGI("Puncher_Main", "System booted.");
}

void motor_init()
{
    ESP_LOGI("Puncher_Main", "Motor initializing...");

    DRIVER_SERIAL.begin(DRIVER_UART_BANDRATE);

    controller_X = new TMC_LEDCMotorController(MOTOR_STEPS, MICROSTEPS_X, xdirPin, xstepPin, xenablePin, X_CONTROL_CHANNEL, &DRIVER_SERIAL, R_SENSE, 0b00);
    controller_Y = new TMC_LEDCMotorController(MOTOR_STEPS, MICROSTEPS_Y, ydirPin, ystepPin, yenablePin, Y_CONTROL_CHANNEL, &DRIVER_SERIAL, R_SENSE, 0b01);
    controller_Z = new TMC_LEDCMotorController(MOTOR_STEPS, MICROSTEPS_Z, zdirPin, zstepPin, zenablePin, Z_CONTROL_CHANNEL, &DRIVER_SERIAL, R_SENSE, 0b10);

    controller_X->begin();  // begin before attach
    controller_Y->begin();
    controller_Z->begin();

    controller_X->setActiveState(LOW);
    controller_Y->setActiveState(LOW);
    controller_Z->setActiveState(LOW);

    scheduler->attachMotors(controller_X, controller_Y, controller_Z);

    ESP_LOGI("Puncher_Main", "Motor initialized!");
}

void lvgl_init()
{
    ESP_LOGI("Puncher_Main", "Starting LVGL...");

    lvgl_ui->setLanguage(scheduler->getDisplayLanguage());
    lvgl_ui->begin();   // begin before attach

    ESP_LOGI("Puncher_Main", "LVGL initialized!");

    ESP_LOGI("Puncher_Main", "Configuring LVGL UI");

    scheduler->attachUI(lvgl_ui);

    ESP_LOGI("Puncher_Main", "LVGL UI ready!");
}

void serial_init()
{
    ESP_LOGI("Puncher_Main", "Starting USB Serial Interface...");

    usbInterface->begin();
    // usbInterface->enableLog();

    ESP_LOGI("Puncher_Main", "USB Serial Interface initialized!");
}

void loop()
{
    vTaskDelete(NULL);
}