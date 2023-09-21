#include "./ui_events.h"
#include "./ui.h"
#include "./LVGL_PuncherUI.h"

void ui_event_Home_Button1_onClick(lv_event_t * e)
{
    // lv_disp_load_scr(ui_Feed_Screen);
}

void ui_event_Home_Button2_onClick(lv_event_t * e)
{

}

void ui_event_Info_Button1_onClick(lv_event_t * e)
{

}

void ui_event_Info_Button2_onClick(lv_event_t * e)
{

}

void ui_event_Feed_Screen_onLoad(lv_event_t * e)
{
    lv_group_focus_obj(ui_Feed_Roller);
    lv_group_focus_freeze(ui_group, true);
    lv_group_set_editing(ui_group, true);
}

void ui_event_Feed_Roller_onKey(lv_event_t * e)
{
    lv_obj_t *roller = e->target;
    int selected = lv_roller_get_selected(roller);
    selected = 4 - selected;
    if (lv_event_get_key(e) == LV_KEY_ENTER)
    {
        lv_group_set_editing(ui_group, true);
    }
    
    lvgl_ui->getScheduler()->feed_paper(selected);
}