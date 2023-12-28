
/**
 * @file lv_port_indev_ctp.h
 *
 */

/*Copy this file as "lv_port_indev_ctp.h" and set this value to "1" to enable content*/
#if 1

#ifndef LV_PORT_INDEV_CTP_H
#define LV_PORT_INDEV_CTP_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl.h"

/*********************
 *      DEFINES
 *********************/
#define CTP_INT     15
#define CTP_RST     16
#define CTP_I2C_SCL 6
#define CTP_I2C_SDA 7

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void lv_port_indev_init(void);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_PORT_INDEV_TEMPL_H*/

#endif /*Disable/Enable content*/
