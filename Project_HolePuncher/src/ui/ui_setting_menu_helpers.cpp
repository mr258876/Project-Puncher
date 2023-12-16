#include "ui_setting_menu_helpers.h"

void _ui_menu_switch_set_value(lv_obj_t *obj, std::any param)
{
    if (std::any_cast<uint8_t>(param))
    {
        lv_obj_add_state(obj, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_clear_state(obj, LV_STATE_CHECKED);
    }
}

void _ui_menu_roller_set_value(lv_obj_t *obj, std::any param)
{
    lv_roller_set_selected(obj, std::any_cast<uint16_t>(param), LV_ANIM_ON);
}

void _ui_menu_dropdown_set_value(lv_obj_t *obj, std::any param)
{
    lv_dropdown_set_selected(obj, std::any_cast<uint16_t>(param));
}

void _ui_menu_spinbox_set_value(lv_obj_t *obj, std::any param)
{
    lv_spinbox_set_value(obj, std::any_cast<int32_t>(param));
}
