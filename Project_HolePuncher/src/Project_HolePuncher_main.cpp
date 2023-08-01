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
static uint8_t *u8g2_buf;
static lv_disp_drv_t disp_drv;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf;

// ---------- Function declarations
void motor_init();
void oled_init();
void lvgl_init();
void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
void disp_rounder_cb(lv_disp_drv_t * disp_drv, lv_area_t *a) ;
void disp_set_px_cb(lv_disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y, lv_color_t color, lv_opa_t opa);

void setup()
{
    Serial.begin(115200);
    ESP_LOGI("main", "Serial interface init.");

    scheduler = new PuncherScheduler();

    motor_init();

    oled_init();
    lvgl_init();
    

    ESP_LOGI("main", "System booted.");
}

void motor_init()
{
    ESP_LOGI("main", "Motor initializing...");

    controller_X = new TMC_LEDCMotorController(MOTOR_STEPS, MICROSTEPS_X, xdirPin, xstepPin, xenablePin, X_CONTROL_CHANNEL, &DRIVER_SERIAL, R_SENSE, 0b00);
    controller_Y = new TMC_LEDCMotorController(MOTOR_STEPS, MICROSTEPS_Y, ydirPin, ystepPin, yenablePin, Y_CONTROL_CHANNEL, &DRIVER_SERIAL, R_SENSE, 0b01);
    controller_Z = new TMC_LEDCMotorController(MOTOR_STEPS, MICROSTEPS_Z, zdirPin, zstepPin, zenablePin, Z_CONTROL_CHANNEL, &DRIVER_SERIAL, R_SENSE, 0b10);

    scheduler->attachMotors(controller_X, controller_Y, controller_Z);

    // controller_X->begin();
    // controller_Y->begin();
    // controller_Z->begin();

    ESP_LOGI("main", "Motor initialized!");
}

void oled_init()
{
    ESP_LOGI("main", "OLED initializing...");

    gfx = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE, U8X8_PIN_NONE, U8X8_PIN_NONE);
    gfx->begin();
    gfx->drawBox(3,7,25,15);
    gfx->sendBuffer();
    u8g2_buf =gfx->getBufferPtr();

    ESP_LOGI("main", "OLED initialized!");
}

void lvgl_init()
{
    ESP_LOGI("main", "Starting LVGL...");

    lv_init();
    
    disp_draw_buf = (lv_color_t *)malloc(128*8);

    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, SCREEN_WIDTH * SCREEN_HEIGHT);

    /* Initialize the display */
    lv_disp_drv_init(&disp_drv);

    /* Change the following line to your display resolution */
    disp_drv.hor_res = SCREEN_WIDTH;
    disp_drv.ver_res = SCREEN_HEIGHT;
    disp_drv.flush_cb = disp_flush;
    disp_drv.rounder_cb = disp_rounder_cb;
    disp_drv.set_px_cb = disp_set_px_cb;
    disp_drv.draw_buf = &draw_buf;
    // disp_drv.direct_mode = true;

    lv_disp_drv_register(&disp_drv);

    /* Initialize input device driver */
    lv_pcnt_encoder_indev_init();

    ui_init();

    LV_LOG_INFO("LVGL Booted.");
}

void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  lv_coord_t w = (area->x2 - area->x1 + 1);
  lv_coord_t h = (area->y2 - area->y1 + 1);

  if (xSemaphoreTake(*LCDMutexptr, portMAX_DELAY) == pdTRUE)
  {
    gfx->drawBitmap(area->x1, area->y1, w/8, h, (uint8_t *)color_p);
    gfx->sendBuffer();
    xSemaphoreGive(*LCDMutexptr);
  }
  lv_disp_flush_ready(disp);
}

void disp_rounder_cb(lv_disp_drv_t * disp_drv, lv_area_t *a) 
{ 
    a->x1 = a->x1 & ~(0x7); 
    a->x2 = a->x2 | (0x7);
}

void disp_set_px_cb(lv_disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y, lv_color_t color, lv_opa_t opa)
{
    buf += buf_w / 8 * y;
    buf += x / 8;
    if(lv_color_brightness(color) > 128) {(*buf) |= (1 << (7 - x % 8));}
    else {(*buf) &= ~(1 << (7 - x % 8));}
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