#include "./ui_events.h"
#include "./ui.h"
#include "./LVGL_PuncherUI.h"
#include "PuncherEvents.h"
#include "esp_log.h"

void ui_event_StartButton(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        lvgl_ui->getScheduler()->start_workload();
    }
}
void ui_event_PauseButton(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        /* TODO */
    }
}
void ui_event_StopButton(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        /* TODO */
    }
}

void ui_event_FeedButton(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        lvgl_ui->getScheduler()->feed_paper_mode(1);
        _ui_screen_change(&ui_FeedScreen, LV_SCR_LOAD_ANIM_FADE_ON, 250, 0, &ui_FeedScreen_screen_init);
    }
}
void ui_event_FeedBackButton(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        _ui_screen_change(&ui_HomeScreen, LV_SCR_LOAD_ANIM_FADE_ON, 250, 0, &ui_HomeScreen_screen_init);
        _ui_roller_set_property(ui_FeedRoller, _UI_ROLLER_PROPERTY_SELECTED_WITH_ANIM, 4);
        lv_event_send(ui_FeedRoller, LV_EVENT_VALUE_CHANGED, NULL);
        lvgl_ui->getScheduler()->feed_paper_mode(0);
    }
}

void ui_event_mcodeTickRate(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"mcode_default_tick_rate", lv_spinbox_get_value(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}

void ui_event_XLeadLength(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"x_lead_length", lv_spinbox_get_value(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_XOperationalSpeed(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"x_operational_speed", lv_spinbox_get_value(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_XLengthType(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"x_length_type", lv_dropdown_get_selected(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_XReverseAxis(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_CLICKED)
    {
        puncher_event_setting_change_t evt = {"x_reverse_axis", (uint8_t)lv_obj_has_state(obj, LV_STATE_CHECKED)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_XOperationalCurrent(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"x_operational_current", lv_spinbox_get_value(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_XIdleBehavior(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"x_idle_behavior", lv_dropdown_get_selected(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_XSleepCurrent(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"x_sleep_current", lv_spinbox_get_value(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_XAutoZreoing(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_CLICKED)
    {
        puncher_event_setting_change_t evt = {"x_auto_zreoing", (uint8_t)lv_obj_has_state(obj, LV_STATE_CHECKED)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_XZeroingTorchThres(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"x_zeroing_torch_thres", lv_spinbox_get_value(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_XZeroingCurrent(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"x_zeroing_current", lv_spinbox_get_value(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_XZeroingSpeed(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"x_zeroing_speed", lv_spinbox_get_value(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_XZeroingPosition(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"x_zeroing_position", lv_spinbox_get_value(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}

void ui_event_YLeadLength(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"y_lead_length", lv_spinbox_get_value(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_YOperationalSpeed(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"y_operational_speed", lv_spinbox_get_value(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_YLengthType(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"y_length_type", lv_dropdown_get_selected(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_YReverseAxis(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"y_reverse_axis", (uint8_t)lv_obj_has_state(obj, LV_STATE_CHECKED)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_YOperationalCurrent(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"y_operational_current", lv_spinbox_get_value(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_YIdleBehavior(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"y_idle_behavior", lv_dropdown_get_selected(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_YSleepCurrent(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"y_sleep_current", lv_spinbox_get_value(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_YAutoZreoing(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"y_auto_zreoing", (uint8_t)lv_obj_has_state(obj, LV_STATE_CHECKED)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_YZeroingTorchThres(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"y_zeroing_torch_thres", lv_spinbox_get_value(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_YZeroingCurrent(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"y_zeroing_current", lv_spinbox_get_value(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_YZeroingSpeed(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"y_zeroing_speed", lv_spinbox_get_value(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_YZeroingPosition(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"y_zeroing_position", lv_spinbox_get_value(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}

void ui_event_ZLeadLength(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"z_lead_length", lv_spinbox_get_value(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_ZOperationalSpeed(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"z_operational_speed", lv_spinbox_get_value(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_ZLengthType(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"z_length_type", lv_dropdown_get_selected(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_ZReverseAxis(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"z_reverse_axis", (uint8_t)lv_obj_has_state(obj, LV_STATE_CHECKED)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_ZOperationalCurrent(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"z_operational_current", lv_spinbox_get_value(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_ZIdleBehavior(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"z_idle_behavior", lv_dropdown_get_selected(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_ZSleepCurrent(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"z_sleep_current", lv_spinbox_get_value(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_ZCaliTarget(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"z_cali_target_bar", lv_spinbox_get_value(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_ZCaliMeasure(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"z_cali_measure_bar", lv_spinbox_get_value(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_ZCaliResidual(lv_event_t *e)
{
    lv_obj_t *obj = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"z_cali_residual", lv_spinbox_get_value(obj)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}

void ui_event_VoltageDropDown(lv_event_t *e)
{
    lv_obj_t *dp = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"power_voltage", lv_dropdown_get_selected(dp)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}

void ui_event_BrightnessSlider(lv_event_t *e)
{
    lv_obj_t *slider = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"display_brightness", lv_slider_get_value(slider)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}
void ui_event_LanguageDropDown(lv_event_t *e)
{
    lv_obj_t *dp = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"display_language", lv_dropdown_get_selected(dp)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}

void ui_event_FeedRoller(lv_event_t *e)
{
    lv_obj_t *roller = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        lvgl_ui->getScheduler()->feed_paper(4 - lv_roller_get_selected(roller));
    }
}