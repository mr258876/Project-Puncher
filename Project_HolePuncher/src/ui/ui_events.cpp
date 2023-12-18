#include "./ui_events.h"
#include "./ui.h"
#include "./LVGL_PuncherUI.h"
#include "PuncherEvents.h"

void ui_event_BrightnessSlider(lv_event_t *e)
{
    lv_obj_t *slider = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        lvgl_ui->setBrightness(lv_slider_get_value(slider));
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

void ui_event_VoltageDropDown(lv_event_t *e)
{
    lv_obj_t *dp = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"power_voltage", lv_dropdown_get_selected(dp)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}