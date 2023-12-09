#include "LVGL_PuncherUI.h"
#include "PuncherConf.h"

#include "ui/ui.h"
#include "ui/lv_port.h"
#include "ui/lv_port_indev_ctp.h"

#include <Arduino.h>

LVGLPuncherUI::LVGLPuncherUI()
{   
    lv_init();

    /* Initialize the display */
    lv_disp_drv_init(&disp_drv);
}

LVGLPuncherUI::~LVGLPuncherUI()
{
}

void LVGLPuncherUI::begin()
{
    lv_port_init();

    /* Initialize input device driver */
    lv_port_indev_init();
    
    ui_init();

    LV_LOG_INFO("LVGL Booted.");

    ledcSetup(LCD_LEDC_CHANNEL, 48000, 8);
    ledcAttachPin(BL_PIN, LCD_LEDC_CHANNEL);
    this->setBrightness(128);
}

void LVGLPuncherUI::alert(ui_alert_msg_t *msg)
{
    
}

void LVGLPuncherUI::drawMenu(std::vector<ui_menu_item_t>, void* ui_param)
{

}

void LVGLPuncherUI::setBrightness(int brightness)
{
    ledcWrite(LCD_LEDC_CHANNEL, brightness);
}

LVGLPuncherUI *lvgl_ui = new LVGLPuncherUI();