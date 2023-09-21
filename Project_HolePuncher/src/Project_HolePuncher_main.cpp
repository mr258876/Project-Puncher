#include <Arduino.h>
#include "PuncherConf.h"
#include "PuncherSemaphore.h"

#include <U8g2lib.h>
#include <lvgl.h>

#include "PuncherScheduler.h"
#include "SerialCommand.h"

#include "MotorController/TMC_LEDCMotorController.h"
#include "ui/LVGL_PuncherUI.h"

static PuncherScheduler *scheduler;

static TMC_LEDCMotorController *controller_X;
static TMC_LEDCMotorController *controller_Y;
static TMC_LEDCMotorController *controller_Z;

static U8G2 *gfx;
static uint8_t *u8g2_buf;
static lv_disp_drv_t disp_drv;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf;

// ---------- Function declarations
void motor_init();
void oled_init();
void lvgl_init();
void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void disp_rounder_cb(lv_disp_drv_t *disp_drv, lv_area_t *a);
void disp_set_px_cb(lv_disp_drv_t *disp_drv, uint8_t *buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y, lv_color_t color, lv_opa_t opa);

void setup()
{
    Serial.begin(115200);
    ESP_LOGI("Puncher_Main", "Serial interface init.");

    scheduler = new PuncherScheduler();

    motor_init();

    oled_init();
    lvgl_init();

    ESP_LOGI("Puncher_Main", "System booted.");
}

void motor_init()
{
    ESP_LOGI("Puncher_Main", "Motor initializing...");

    DRIVER_SERIAL.begin(DRIVER_UART_BANDRATE);

    controller_X = new TMC_LEDCMotorController(MOTOR_STEPS, MICROSTEPS_X, xdirPin, xstepPin, xenablePin, X_CONTROL_CHANNEL, &DRIVER_SERIAL, R_SENSE, 0b00);
    controller_Y = new TMC_LEDCMotorController(MOTOR_STEPS, MICROSTEPS_Y, ydirPin, ystepPin, yenablePin, Y_CONTROL_CHANNEL, &DRIVER_SERIAL, R_SENSE, 0b01);
    controller_Z = new TMC_LEDCMotorController(MOTOR_STEPS, MICROSTEPS_Z, zdirPin, zstepPin, zenablePin, Z_CONTROL_CHANNEL, &DRIVER_SERIAL, R_SENSE, 0b10);

    scheduler->attachMotors(controller_X, controller_Y, controller_Z);

    controller_X->begin();
    controller_Y->begin();
    controller_Z->begin();

    ESP_LOGI("Puncher_Main", "Motor initialized!");
}

void oled_init()
{
    ESP_LOGI("Puncher_Main", "OLED initializing...");

    gfx = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE, U8X8_PIN_NONE, U8X8_PIN_NONE);
    gfx->begin();
    gfx->drawBox(3, 7, 25, 15);
    gfx->sendBuffer();
    u8g2_buf = gfx->getBufferPtr();

    ESP_LOGI("Puncher_Main", "OLED initialized!");
}

void lvgl_init()
{
    ESP_LOGI("Puncher_Main", "Starting LVGL...");

    lvgl_ui->disp_drv.flush_cb = disp_flush;
    lvgl_ui->disp_drv.rounder_cb = disp_rounder_cb;
    lvgl_ui->disp_drv.set_px_cb = disp_set_px_cb;

    lvgl_ui->begin();
    scheduler->attachUI(lvgl_ui);

    ESP_LOGI("Puncher_Main", "LVGL initialized!");
}

void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    lv_coord_t w = (area->x2 - area->x1 + 1);
    lv_coord_t h = (area->y2 - area->y1 + 1);

    if (xSemaphoreTake(*LCDMutexptr, portMAX_DELAY) == pdTRUE)
    {
        gfx->drawBitmap(area->x1, area->y1, w / 8, h, (uint8_t *)color_p);
        gfx->sendBuffer();
        xSemaphoreGive(*LCDMutexptr);
    }
    lv_disp_flush_ready(disp);
}

void disp_rounder_cb(lv_disp_drv_t *disp_drv, lv_area_t *a)
{
    a->x1 = a->x1 & ~(0x7);
    a->x2 = a->x2 | (0x7);
}

void disp_set_px_cb(lv_disp_drv_t *disp_drv, uint8_t *buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y, lv_color_t color, lv_opa_t opa)
{
    buf += buf_w / 8 * y;
    buf += x / 8;
    if (lv_color_brightness(color) > 128)
    {
        (*buf) |= (1 << (7 - x % 8));
    }
    else
    {
        (*buf) &= ~(1 << (7 - x % 8));
    }
}

void loop()
{
    scheduler->tick();
    if (xSemaphoreTake(LVGLMutex, portMAX_DELAY) == pdTRUE)
    {
        lv_timer_handler(); /* let the GUI do its work */
        xSemaphoreGive(LVGLMutex);
    }
    vTaskDelay(5);
}