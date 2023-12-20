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
        uint32_t evt_bits = xEventGroupWaitBits(ui->evt_group, 0xFF, pdTRUE, pdFALSE, portMAX_DELAY);

        if (evt_bits & OnEventCode)
        {
            // TODO
        }

        if (evt_bits & OnSettingValueChange)
        {
            xQueueReceive(ui->setting_value_queue, evt_tmp, portMAX_DELAY);
            ui->handleSettingValueChange((puncher_event_setting_change_t *)evt_tmp);
        }

        /* If there's still data in queue */
        if (uxQueueMessagesWaiting(ui->event_code_queue))  xEventGroupSetBits(ui->evt_group, OnEventCode);
        if (uxQueueMessagesWaiting(ui->setting_value_queue))  xEventGroupSetBits(ui->evt_group, OnSettingValueChange);
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

    this->event_code_queue = xQueueCreate(16, sizeof(puncher_event_code_t));
    this->setting_value_queue = xQueueCreate(16, sizeof(puncher_event_setting_change_t));
    this->evt_group = xEventGroupCreate();
    xTaskCreate(evtQueueHandleLoop, "SchedulerEvtLoop", 8192, this, 3, NULL);
}

void LVGLPuncherUI::onStatusCode(puncher_event_code_t *msg)
{
    xQueueSend(event_code_queue, msg, portMAX_DELAY);
    xEventGroupSetBits(evt_group, OnEventCode);
}

void LVGLPuncherUI::onSettingValueChange(puncher_event_setting_change_t *msg)
{
    xQueueSend(setting_value_queue, msg, portMAX_DELAY);
    xEventGroupSetBits(evt_group, OnSettingValueChange);
}

void LVGLPuncherUI::attachScheduler(PuncherSchedulerInterface *p_scheduler)
{
    this->scheduler = p_scheduler;
    p_scheduler->get_setting_values(this);
}

void LVGLPuncherUI::handleSettingValueChange(puncher_event_setting_change_t *msg)
{
    ESP_LOGI("LVGLPuncherUI", "Value %s has changed.", msg->item_name);
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