#include "LVGL_PuncherUI.h"
#include "PuncherConf.h"

#include "ui/ui.h"
#include "ui/ui_setting_menu_mapping.h"
#include "ui/lv_port.h"
#include "ui/lv_port_indev_ctp.h"
#include "PuncherSemaphore.h"

#include <Arduino.h>

#include <any>

#define MAXIUM(x, y) ((x) > (y) ? (x) : (y))

static const char *TAG = "LVGLPuncherUI";

void lvglLoop(void *param)
{
    LVGLPuncherUI *ui = (LVGLPuncherUI *)param;

    ui_init();
    ui_menu_ptr_update();
    xEventGroupSetBits(ui->evt_group, (1 << 8));

    uint8_t evt_tmp[MAXIUM(sizeof(puncher_status_t), sizeof(puncher_event_t))];
    uint32_t evt_bits = 0;

    while (1)
    {
        // Process Events
        evt_bits = xEventGroupWaitBits(ui->evt_group, 0xFF, pdFALSE, pdFALSE, pdMS_TO_TICKS(5));

        // make sure we have done all changes before rendering
        while (evt_bits)
        {
            xEventGroupClearBits(ui->evt_group, 0xFF);

            if (evt_bits & OnEventCode)
            {
                xQueueReceive(ui->event_code_queue, evt_tmp, portMAX_DELAY);
                ui->handleEventCodeChange((puncher_status_t *)evt_tmp);
            }

            if (evt_bits & OnSettingValueChange)
            {
                xQueueReceive(ui->setting_value_queue, evt_tmp, portMAX_DELAY);
                ui->handleSettingValueChange((puncher_event_setting_change_t *)evt_tmp);
            }

            /* If there's still data in queue */
            if (uxQueueMessagesWaiting(ui->event_code_queue))
                xEventGroupSetBits(ui->evt_group, OnEventCode);
            if (uxQueueMessagesWaiting(ui->setting_value_queue))
                xEventGroupSetBits(ui->evt_group, OnSettingValueChange);

            evt_bits = xEventGroupGetBits(ui->evt_group);
        }

        // Handle UI
        xSemaphoreTake(LVGLMutex, portMAX_DELAY);
        {
            lv_timer_handler(); /* let the GUI do its work */
        }
        xSemaphoreGive(LVGLMutex);
    }

    vTaskDelete(NULL);
}

LVGLPuncherUI::LVGLPuncherUI()
{
}

LVGLPuncherUI::~LVGLPuncherUI()
{
}

void LVGLPuncherUI::begin()
{
    this->event_code_queue = xQueueCreate(16, sizeof(puncher_status_t));
    this->setting_value_queue = xQueueCreate(16, sizeof(puncher_event_setting_change_t));
    this->evt_group = xEventGroupCreate();

    lv_port_init();

    /* Initialize input device driver */
    lv_port_indev_init();

    /* Initialize NotoSans */
    lv_font_notosans_init();

    /* Initialize Multilanguage */
    lv_i18n_init(lv_i18n_language_pack);
    lv_i18n_set_locale(lv_i18n_language_pack[lang_id]->locale_name);

    xTaskCreatePinnedToCore(lvglLoop, "lvglLoop", 32768, this, 1, NULL, 1);
    xEventGroupWaitBits(evt_group, (1 << 8), pdTRUE, pdTRUE, portMAX_DELAY);

    LV_LOG_INFO("LVGL Booted.");

    ledcAttachChannel(BL_PIN, 48000, 8, LCD_LEDC_CHANNEL);
    this->setBrightness(1);
}

void LVGLPuncherUI::onStatusCode(puncher_status_t *msg)
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

void LVGLPuncherUI::handleEventCodeChange(puncher_status_t *msg)
{
    ESP_LOGI(TAG, "Scheduler status update!");
    xSemaphoreTake(LVGLMutex, portMAX_DELAY);
    if (msg->basic_status.status_flags.is_running)
    {
        lv_label_set_text_fmt(ui_Label5, "%lu/%lu", msg->finished_length, msg->task_length);

        lv_obj_add_flag(ui_FeedButton, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_SettingButton, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_StartButton, LV_OBJ_FLAG_HIDDEN);

        lv_obj_clear_flag(ui_PauseButton, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_StopButton, LV_OBJ_FLAG_HIDDEN);
    }
    else if (msg->basic_status.status_flags.has_mission)
    {
        lv_label_set_text(ui_Label5, _("Ready"));
        lv_obj_clear_flag(ui_StartButton, LV_OBJ_FLAG_HIDDEN);
    }
    else if (msg->basic_status.status_flags.is_transmitting_data)
    {
        lv_label_set_text(ui_Label5, _("Receiving Data"));
    }
    else
    {
        lv_label_set_text(ui_Label5, _("Idle"));

        lv_obj_add_flag(ui_StartButton, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_PauseButton, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_StopButton, LV_OBJ_FLAG_HIDDEN);

        lv_obj_clear_flag(ui_FeedButton, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_SettingButton, LV_OBJ_FLAG_HIDDEN);

        lv_label_set_text(ui_Home_TipsLabel, _("_tips_1"));
    }

    if (msg->basic_status.status_flags.has_error)
    {
        lv_label_set_text(ui_Label5, _("Error Occured"));

        lv_obj_add_flag(ui_FeedButton, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_SettingButton, LV_OBJ_FLAG_HIDDEN);

        std::string s;
        if (msg->basic_status.status_flags.driver_err_x || msg->basic_status.status_flags.driver_err_y || msg->basic_status.status_flags.driver_err_z)
        {
            if (msg->basic_status.status_flags.driver_err_x) s.append("X ");
            if (msg->basic_status.status_flags.driver_err_y) s.append("Y ");
            if (msg->basic_status.status_flags.driver_err_z) s.append("Z ");
            s.append(_("_driver_err_template_cpp_tips"));
            s.append("\n");
        }
        if (msg->basic_status.status_flags.motor_err_x || msg->basic_status.status_flags.motor_err_y || msg->basic_status.status_flags.motor_err_z)
        {
            if (msg->basic_status.status_flags.motor_err_x) s.append("X ");
            if (msg->basic_status.status_flags.motor_err_y) s.append("Y ");
            if (msg->basic_status.status_flags.motor_err_z) s.append("Z ");
            s.append(_("_motor_err_template_cpp_tips"));
            s.append("\n");
        }
        if (msg->basic_status.status_flags.power_err)
        {
            s.append(_("_power_err_template_tips"));
            s.append("\n");
        }
        lv_label_set_text(ui_Home_TipsLabel, s.c_str());
    }

    // display ETA
    if (msg->basic_status.status_flags.is_running)
    {
        lv_obj_clear_flag(ui_Label6, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_Label7, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_Home_TipsLabel, LV_OBJ_FLAG_HIDDEN);

        lv_label_set_text_fmt(ui_Label7, "%02lld:%02lld:%02lld", (msg->ETA / 3600), (msg->ETA % 3600) / 60, (msg->ETA % 60));
        time_t *timer_eta = (time_t *)ui_ETA_timer->user_data;
        *timer_eta = msg->ETA;
        lv_timer_reset(ui_ETA_timer);
        lv_timer_resume(ui_ETA_timer);
    }
    else
    {
        lv_obj_add_flag(ui_Label6, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(ui_Label7, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_Home_TipsLabel, LV_OBJ_FLAG_HIDDEN);
        lv_timer_pause(ui_ETA_timer);
    }

    xSemaphoreGive(LVGLMutex);
}

void LVGLPuncherUI::handleSettingValueChange(puncher_event_setting_change_t *msg)
{
    ESP_LOGI(TAG, "Value %s has changed.", msg->item_name);
    ui_menu_on_val_change(msg);
}

void LVGLPuncherUI::setBrightness(int brightness)
{
    ledcWrite(BL_PIN, brightness);
}

void LVGLPuncherUI::setLanguage(uint16_t lang_id)
{
    this->lang_id = lang_id;
}

LVGLPuncherUI *lvgl_ui = new LVGLPuncherUI();