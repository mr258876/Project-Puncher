/**
 * @file lv_port_ctp.cpp
 *
 */

/*Copy this file as "lv_port_indev_ctp.cpp" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_indev_ctp.h"
#include "lvgl.h"

#include <Wire.h>
#include <FT6X36.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "PuncherSemaphore.h"

/*********************
 *      DEFINES
 *********************/
#define TouchEvent (1 << 0) // 1

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void touchpad_init(void);
static void touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);

static void TaskProcessTouch(void *pvParameters);
static void IRAM_ATTR touch_isr();
static void on_touch(TPoint p, TEvent e);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_indev_t *indev_touchpad;

FT6X36 ctp(&Wire1, CTP_INT);
EventGroupHandle_t eg;

bool has_touch;
lv_coord_t touch_x = 0;
lv_coord_t touch_y = 0;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_indev_init(void)
{
    static lv_indev_drv_t indev_drv;

    /*------------------
     * Touchpad
     * -----------------*/

    /*Initialize your touchpad if you have*/
    touchpad_init();

    /*Register a touchpad input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    indev_touchpad = lv_indev_drv_register(&indev_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*------------------
 * Touchpad
 * -----------------*/

/*Initialize your touchpad*/
static void touchpad_init(void)
{
    /*Your code comes here*/
    eg = xEventGroupCreate();

    
    xSemaphoreTake(I2C0Mutex, portMAX_DELAY);
    {
        Wire1.begin(CTP_I2C_SDA, CTP_I2C_SCL, 400000U);
        ctp.begin();

        ctp.registerIsrHandler(touch_isr);
        ctp.registerTouchHandler(on_touch);
    }
    xSemaphoreGive(I2C0Mutex);

    pinMode(CTP_INT, INPUT_PULLUP);
    attachInterrupt(CTP_INT, touch_isr, FALLING);

    xTaskCreatePinnedToCore(TaskProcessTouch, "fTaskProcessTouch", 4096, NULL, 3, NULL, 1);
}

/*Will be called by the library to read the touchpad*/
static void touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{

    /*Save the pressed coordinates and the state*/
    if (has_touch)
    {
        data->state = LV_INDEV_STATE_PR;
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }

    /*Set the last pressed coordinates*/
    data->point.x = touch_x;
    data->point.y = touch_y;
}

void TaskProcessTouch(void *pvParameters)
{
    for (;;)
    {
        xEventGroupWaitBits(eg, TouchEvent, pdTRUE, pdTRUE, portMAX_DELAY);
        {
            xSemaphoreTake(I2C0Mutex, portMAX_DELAY);
            {
                ctp.processTouch();
            }
            xSemaphoreGive(I2C0Mutex);
            ESP_LOGI("ctp", "touch!");
        }
    }
}

void IRAM_ATTR touch_isr()
{
    BaseType_t xHigherPriorityTaskWoken;
    xEventGroupSetBitsFromISR(eg, TouchEvent, &xHigherPriorityTaskWoken);
}

void on_touch(TPoint p, TEvent e)
{
    if (e == TEvent::TouchStart || e == TEvent::DragStart)
    {
        has_touch = true;
    }
    else if (e == TEvent::TouchEnd || e == TEvent::DragEnd)
    {
        has_touch = false;
    }

    // x, y is in opposite position
    touch_x = p.y;
    touch_y = 240 - p.x;
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
