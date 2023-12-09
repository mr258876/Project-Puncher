#include "./ui_events.h"
#include "./ui.h"
#include "./LVGL_PuncherUI.h"

void ui_event_BrightnessSlider(lv_event_t * e)
{
    lv_obj_t *slider = e->target;
    
    if (e->code == LV_EVENT_VALUE_CHANGED)
    {
        lvgl_ui->setBrightness(lv_slider_get_value(slider));
    }
    
}