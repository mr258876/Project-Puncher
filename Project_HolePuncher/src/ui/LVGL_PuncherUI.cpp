#include "LVGL_PuncherUI.h"
#include "PuncherConf.h"

#include "ui/ui.h"
#include "ui/ui_setting_menu_mapping.h"
#include "ui/lv_port.h"
#include "ui/lv_port_indev_ctp.h"

#include <Arduino.h>

#include <any>

void evtQueueHandleLoop(void *param)
{
    LVGLPuncherUI *ui = (LVGLPuncherUI *)param;
    uint8_t evt_tmp[sizeof(puncher_event_t)];
    while (1)
    {
        xQueueReceive(ui->evt_queue, evt_tmp, portMAX_DELAY);
        ui->handleEvent((puncher_event_t *)evt_tmp);
    }
}

void LVGLPuncherUI::handleEvent(puncher_event_t *msg)
{
    ESP_LOGD("LVGLPuncherUI", "onEvent: %d", msg->code);
    if (msg->code == PUNCHER_EVENT_SETTING_VALUE_CHANGED)
    {
        ESP_LOGD("LVGLPuncherUI", "Setting value changed");
        puncher_event_setting_change_t *data = std::any_cast<puncher_event_setting_change_t *>(msg->data);
        this->onSettingValueChange(data);
        delete data;
    }
}

LVGLPuncherUI::LVGLPuncherUI()
{
}

LVGLPuncherUI::~LVGLPuncherUI()
{
}

void LVGLPuncherUI::begin()
{
    lv_port_init();

    /* Initialize input device driver */
    lv_port_indev_init();

    /* Initialize Multilanguage */
    lv_i18n_init(lv_i18n_language_pack);
    lv_i18n_set_locale(lv_i18n_language_pack[lang_id]->locale_name);

    ui_init();
    ui_menu_ptr_update();

    LV_LOG_INFO("LVGL Booted.");

    ledcSetup(LCD_LEDC_CHANNEL, 48000, 8);
    ledcAttachPin(BL_PIN, LCD_LEDC_CHANNEL);
    this->setBrightness(1);

    this->evt_queue = xQueueCreate(16, sizeof(puncher_event_t));
    xTaskCreate(evtQueueHandleLoop, "SchedulerEvtLoop", 8192, this, 3, NULL);
}

void LVGLPuncherUI::onEvent(puncher_event_t *msg)
{
    xQueueSend(evt_queue, msg, portMAX_DELAY);
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

void LVGLPuncherUI::setLanguage(uint16_t lang_id)
{
    this->lang_id = lang_id;
}

LVGLPuncherUI *lvgl_ui = new LVGLPuncherUI();