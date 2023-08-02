/**
 * @file lv_port_indev_templ.c
 *
 */

/*Copy this file as "lv_port_indev.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_encoder.h"
#include <lvgl.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void encoder_init(void);
static void encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static void encoder_a_handler(void* param);
static void encoder_b_handler(void* param);
static void encoder_btn_handler(void* param);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_indev_t * indev_encoder;

static int32_t encoder_diff;
static lv_indev_state_t encoder_state;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_encoder_indev_init(void)
{
    /**
     * Here you will find example implementation of input devices supported by LittelvGL:
     *  - Touchpad
     *  - Mouse (with cursor support)
     *  - Keypad (supports GUI usage only with key)
     *  - Encoder (supports GUI usage only with: left, right, push)
     *  - Button (external buttons to press points on the screen)
     *
     *  The `..._read()` function are only examples.
     *  You should shape them according to your hardware
     */

    static lv_indev_drv_t indev_drv;

    /*------------------
     * Encoder
     * -----------------*/

    /*Initialize your encoder if you have*/
    encoder_init();

    /*Register a encoder input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = encoder_read;
    indev_encoder = lv_indev_drv_register(&indev_drv);

    /*Later you should create group(s) with `lv_group_t * group = lv_group_create()`,
     *add objects to the group with `lv_group_add_obj(group, obj)`
     *and assign this input device to group to navigate in it:
     *`lv_indev_set_group(indev_encoder, group);`*/
    ui_group = lv_group_create();
    lv_group_set_default(ui_group);
    lv_indev_set_group(indev_encoder, ui_group);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*------------------
 * Encoder
 * -----------------*/

/*Initialize your keypad*/
static void encoder_init(void)
{
    gpio_set_direction(ENCODER_A, GPIO_MODE_INPUT);
    gpio_set_direction(ENCODER_B, GPIO_MODE_INPUT);
    gpio_set_direction(ENCODER_BTN, GPIO_MODE_INPUT);

    gpio_set_intr_type(ENCODER_A, GPIO_INTR_NEGEDGE);
    gpio_set_intr_type(ENCODER_B, GPIO_INTR_NEGEDGE);

    gpio_install_isr_service(ESP_INTR_FLAG_SHARED);
    gpio_isr_handler_add(ENCODER_A, encoder_a_handler, NULL);
    gpio_isr_handler_add(ENCODER_B, encoder_b_handler, NULL);
}

/*Will be called by the library to read the encoder*/
static void encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    encoder_state = (gpio_get_level(ENCODER_BTN)) ? LV_INDEV_STATE_REL : LV_INDEV_STATE_PR;

    data->enc_diff = encoder_diff;
    data->state = encoder_state;
    encoder_diff = 0;
}

/*Call this function in an interrupt to process encoder events (turn, press)*/
static void encoder_a_handler(void* param)
{
    encoder_diff += (gpio_get_level(ENCODER_B)) ? 1 : -1;
}

static void encoder_b_handler(void* param)
{
    encoder_diff += (gpio_get_level(ENCODER_A)) ? -1 : 1;
}

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
