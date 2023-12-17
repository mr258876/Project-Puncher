#include "LVGL_PuncherUI.h"
#include "PuncherConf.h"

#include "ui/ui.h"
#include "ui/ui_setting_menu_mapping.h"
#include "ui/lv_port.h"
#include "ui/lv_port_indev_ctp.h"

#include <Arduino.h>

#include <any>

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
    ui_menu_ptr_update();

    LV_LOG_INFO("LVGL Booted.");

    ledcSetup(LCD_LEDC_CHANNEL, 48000, 8);
    ledcAttachPin(BL_PIN, LCD_LEDC_CHANNEL);
    this->setBrightness(128);
}

void LVGLPuncherUI::onEvent(puncher_event_t *msg)
{
    ESP_LOGI("LVGLPuncherUI", "onEvent: %d", msg->code);
    if (msg->code == PUNCHER_EVENT_SETTING_VALUE_CHANGED)
    {
        ESP_LOGI("LVGLPuncherUI", "Setting value changed");
        this->onSettingValueChange(std::any_cast<puncher_event_setting_change_t *>(msg->data));
    }
}

void LVGLPuncherUI::attachScheduler(PuncherSchedulerInterface *p_scheduler)
{
    this->scheduler = p_scheduler;
    p_scheduler->get_setting_values(this);
}

void LVGLPuncherUI::onSettingValueChange(puncher_event_setting_change_t *msg)
{
    ui_menu_on_val_change(msg);
}

void LVGLPuncherUI::setBrightness(int brightness)
{
    ledcWrite(LCD_LEDC_CHANNEL, brightness);
}

LVGLPuncherUI *lvgl_ui = new LVGLPuncherUI();