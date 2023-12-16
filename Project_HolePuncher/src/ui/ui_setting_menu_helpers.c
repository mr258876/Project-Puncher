#include "ui_setting_menu_helpers.h"

void _ui_menu_switch_set_value(lv_obj_t *obj, void *param)
{
    if (*((int *)param))
    {
        lv_obj_add_state(obj, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_clear_state(obj, LV_STATE_CHECKED);
    }
}

void _ui_menu_roller_set_value(lv_obj_t *obj, void *param)
{
    lv_roller_set_selected(obj, *((int *)param), LV_ANIM_ON);
}

void _ui_menu_dropdown_set_value(lv_obj_t *obj, void *param)
{
    lv_dropdown_set_selected(obj, *((int *)param));
}

void _ui_menu_spinbox_set_value(lv_obj_t *obj, void *param)
{
    lv_spinbox_set_value(obj, *((int *)param));
}
