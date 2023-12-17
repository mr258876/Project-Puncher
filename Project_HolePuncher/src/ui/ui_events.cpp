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

void ui_event_VoltageDropDown(lv_event_t *e)
{
    lv_obj_t *dp = e->target;

    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        puncher_event_setting_change_t evt = {"power_voltage", lv_dropdown_get_selected(dp)};
        lvgl_ui->getScheduler()->set_setting_value(&evt);
    }
}