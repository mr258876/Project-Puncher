#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_err.h"

#define LCD_WIDTH       (320)
#define LCD_HEIGHT      (240)

#define LCD_BUF_HEIGHT  (240)

#define SCLK_PIN        (12)
#define MOSI_PIN        (11)
#define CS_PIN          (10)
#define DC_PIN          (17)
#define RST_PIN         (8)

#define SPI_PORT        (SPI2_HOST)
#define SPI_CLK         (120*1000*1000)

#define LCD_DIRECTION   (90)

esp_err_t lv_port_init(void);

#ifdef __cplusplus
}
#endif