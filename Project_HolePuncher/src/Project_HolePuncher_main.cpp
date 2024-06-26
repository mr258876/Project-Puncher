#include <Arduino.h>
#include "PuncherConf.h"
#include "PuncherSemaphore.h"

#include <lvgl.h>

#include "PuncherScheduler.h"
#include "PuncherMotors.h"

#include "MotorController/TMC_LEDCMotorController.h"
#include "PositionSensor/AS5600PositionSensor.h"
#include "ui/LVGL_PuncherUI.h"
#include "SerialInterface/SerialInterface.h"
#include "BLEInterface/BLEInterface.h"
#include "esp_vfs_fat.h"

static PuncherScheduler *scheduler;
static PowerManager *pm;
static wl_handle_t wl_handle_flash = WL_INVALID_HANDLE;

// ---------- Function declarations
void init_spiram();
void pm_init();
void motor_init();
void encoder_init();
void lvgl_init();
void serial_init();
void ble_init();
void fatfs_init();

void setup()
{
    ESP_LOGI("Puncher_Main", "Initializing...");

    scheduler = new PuncherScheduler();

    pm_init(); // Init power manager first or voltage acquire will fail

    scheduler->beginNVS();

    // serial_init();

    motor_init();

    encoder_init();

    ble_init();

    fatfs_init(); // must before lvgl_init

    lvgl_init();

    scheduler->begin();

    ESP_LOGI("Puncher_Main", "System booted.");
}

void pm_init()
{
    ESP_LOGI("Puncher_Main", "Power initializing...");

    pm = new PowerManager(CTP_I2C_SCL, CTP_I2C_SDA, PCF8574_INT);
    pm->begin();

    scheduler->attachPower(pm);

    ESP_LOGI("Puncher_Main", "Power initialized!");
}

void motor_init()
{
    ESP_LOGI("Puncher_Main", "Motor initializing...");

    DRIVER_SERIAL.begin(DRIVER_UART_BANDRATE);

    controller_X = new TMC_LEDCMotorController(MOTOR_STEPS, MICROSTEPS_X, xdirPin, xstepPin, xenablePin, xdiagPin, X_CONTROL_CHANNEL, &DRIVER_SERIAL, R_SENSE, 0b00);
    controller_Y = new TMC_LEDCMotorController(MOTOR_STEPS, MICROSTEPS_Y, ydirPin, ystepPin, yenablePin, ydiagPin, Y_CONTROL_CHANNEL, &DRIVER_SERIAL, R_SENSE, 0b01);
    controller_Z = new TMC_LEDCMotorController(MOTOR_STEPS, MICROSTEPS_Z, zdirPin, zstepPin, zenablePin, zdiagPin, Z_CONTROL_CHANNEL, &DRIVER_SERIAL, R_SENSE, 0b10);

    controller_X->begin(); // begin before attach
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
    lvgl_ui->begin(); // begin before attach

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

    scheduler->attachUI(usbInterface);

    ESP_LOGI("Puncher_Main", "USB Serial Interface initialized!");
}

void ble_init()
{
    ESP_LOGI("Puncher_Main", "Starting BLE Interface...");

    bleInterface->begin();

    scheduler->attachUI(bleInterface);

    ESP_LOGI("Puncher_Main", "BLE Interface initialized!");
}

void encoder_init()
{
    ESP_LOGI("Puncher_Main", "Encoder initializing...");

    switch (scheduler->getZencoderType())
    {
    case 0:
        Wire.begin(I2C0_SDA, I2C0_SCL, 400000U);

        sensor_Z = new AS5600PositionSensor(&Wire, &I2C0Mutex);

        scheduler->attachPositionSensors(NULL, NULL, sensor_Z);
        break;

    default:
        break;
    }

    ESP_LOGI("Puncher_Main", "Encoder initialized!");
}

void fatfs_init()
{
    ESP_LOGI("Puncher_Main", "Mounting flash...");

    const esp_vfs_fat_mount_config_t mount_cfg = {
        .format_if_mount_failed = false,
        .max_files = 4,
        .allocation_unit_size = CONFIG_WL_SECTOR_SIZE,
    };

    ESP_ERROR_CHECK(esp_vfs_fat_spiflash_mount(FLASH_MOUNT_POINT, "fatfs", &mount_cfg, &wl_handle_flash));

    ESP_LOGI("Puncher_Main", "Flash mounted!");
}

void loop()
{
    vTaskDelete(NULL);
}