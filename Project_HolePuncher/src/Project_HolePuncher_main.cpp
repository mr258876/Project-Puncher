#include <Arduino.h>
#include "PuncherConf.h"
#include "PuncherSemaphore.h"

#include <Wire.h>
#include <AS5600.h>

#include <WiFi.h>

#include <U8g2lib.h>
#include <lvgl.h>
#include "ui/ui.h"
#include "ui/lv_port_encoder.h"

#include "PuncherScheduler.h"
#include "SerialCommand.h"

#include "MotorController/TMC_LEDCMotorController.h"

static PuncherScheduler *scheduler;

static TMC_LEDCMotorController *controller_X;
static TMC_LEDCMotorController *controller_Y;
static TMC_LEDCMotorController *controller_Z;

static U8G2 *gfx;
static lv_disp_drv_t disp_drv;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf;

// ---------- Function declarations
void motor_init();
void oled_init();
void lvgl_init();
void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

void setup()
{
    Serial.begin(115200);
    Serial.println("1");

    scheduler = new PuncherScheduler();

    // motor_init();

    oled_init();
    lvgl_init();
    Serial.println("2");
}

void motor_init()
{
    controller_X = new TMC_LEDCMotorController(MOTOR_STEPS, MICROSTEPS_X, xdirPin, xstepPin, xenablePin, X_CONTROL_CHANNEL, &DRIVER_SERIAL, R_SENSE, 0b00);
    controller_Y = new TMC_LEDCMotorController(MOTOR_STEPS, MICROSTEPS_Y, ydirPin, ystepPin, yenablePin, Y_CONTROL_CHANNEL, &DRIVER_SERIAL, R_SENSE, 0b01);
    controller_Z = new TMC_LEDCMotorController(MOTOR_STEPS, MICROSTEPS_Z, zdirPin, zstepPin, zenablePin, Z_CONTROL_CHANNEL, &DRIVER_SERIAL, R_SENSE, 0b10);

    scheduler->attachMotors(controller_X, controller_Y, controller_Z);

    controller_X->begin();
    controller_Y->begin();
    controller_Z->begin();
}

void oled_init()
{
    gfx = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE, U8X8_PIN_NONE, U8X8_PIN_NONE);
    gfx->begin();
}

void lvgl_init()
{
    lv_init();

    disp_draw_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * SCREEN_WIDTH * SCREEN_HEIGHT, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, SCREEN_WIDTH * SCREEN_HEIGHT);

    /* Initialize the display */
    lv_disp_drv_init(&disp_drv);

    /* Change the following line to your display resolution */
    disp_drv.hor_res = SCREEN_WIDTH;
    disp_drv.ver_res = SCREEN_HEIGHT;
    disp_drv.flush_cb = disp_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.sw_rotate = 0;

    lv_disp_drv_register(&disp_drv);

    /* Initialize input device driver */
    lv_port_indev_init();

    ui_init();

    ESP_LOGI("LVGL", "LVGL Booted.");
}

void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  lv_coord_t w = (area->x2 - area->x1 + 1);
  lv_coord_t h = (area->y2 - area->y1 + 1);

  if (xSemaphoreTake(*LCDMutexptr, portMAX_DELAY) == pdTRUE)
  {
    gfx->drawBitmap(area->x1, area->y1, w, h, (uint8_t *)&color_p->full);
    xSemaphoreGive(*LCDMutexptr);
  }
  lv_disp_flush_ready(disp);
}

void loop()
{
    scheduler->tick();
    if (xSemaphoreTake(LVGLMutex, portMAX_DELAY) == pdTRUE)
    {
      lv_timer_handler();   /* let the GUI do its work */
      xSemaphoreGive(LVGLMutex);
    }
    vTaskDelay(5);
}