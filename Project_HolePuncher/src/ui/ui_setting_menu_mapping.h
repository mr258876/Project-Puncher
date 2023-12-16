#ifndef _UI_SETTING_MENU_MAPPING_H_
#define _UI_SETTING_MENU_MAPPING_H_

#include "lvgl.h"
#include "ui_setting_menu.h"
#include "ui_setting_menu_helpers.h"
#include <any>

struct ui_setting_menu_mapping_t
{
    const char *obj_name;
    lv_obj_t *obj;
    void (*call_back)(lv_obj_t *obj, std::any param);
};


const ui_setting_menu_mapping_t setting_mapping[] = {
    {"wifi_switch", ui_setting_wifi_switch, _ui_menu_switch_set_value},

    {"x_lead_length", ui_setting_x_lead_length, _ui_menu_spinbox_set_value},
    {"x_operational_speed", ui_setting_x_operational_speed, _ui_menu_spinbox_set_value},
    {"x_reverse_axis", ui_setting_x_reverse_axis, _ui_menu_switch_set_value},
    {"x_operational_current", ui_setting_x_operational_current, _ui_menu_spinbox_set_value},
    {"x_idle_behavior", ui_setting_x_idle_behavior, _ui_menu_dropdown_set_value},
    {"x_sleep_current", ui_setting_x_sleep_current, _ui_menu_spinbox_set_value},
    {"x_auto_zreoing", ui_setting_x_auto_zreoing, _ui_menu_switch_set_value},
    {"x_zeroing_torch_thres", ui_setting_x_zeroing_torch_thres, _ui_menu_spinbox_set_value},
    {"x_zeroing_current", ui_setting_x_zeroing_current, _ui_menu_spinbox_set_value},
    {"x_zeroing_speed", ui_setting_x_zeroing_speed, _ui_menu_spinbox_set_value},

    {"y_lead_length", ui_setting_y_lead_length, _ui_menu_spinbox_set_value},
    {"y_operational_speed", ui_setting_y_operational_speed, _ui_menu_spinbox_set_value},
    {"y_reverse_axis", ui_setting_y_reverse_axis, _ui_menu_switch_set_value},
    {"y_operational_current", ui_setting_y_operational_current, _ui_menu_spinbox_set_value},
    {"y_idle_behavior", ui_setting_y_idle_behavior, _ui_menu_dropdown_set_value},
    {"y_sleep_current", ui_setting_y_sleep_current, _ui_menu_spinbox_set_value},
    {"y_auto_zreoing", ui_setting_y_auto_zreoing, _ui_menu_switch_set_value},
    {"y_zeroing_torch_thres", ui_setting_y_zeroing_torch_thres, _ui_menu_spinbox_set_value},
    {"y_zeroing_current", ui_setting_y_zeroing_current, _ui_menu_spinbox_set_value},
    {"y_zeroing_speed", ui_setting_y_zeroing_speed, _ui_menu_spinbox_set_value},

    {"z_lead_length", ui_setting_z_lead_length, _ui_menu_spinbox_set_value},
    {"z_operational_speed", ui_setting_z_operational_speed, _ui_menu_spinbox_set_value},
    {"z_reverse_axis", ui_setting_z_reverse_axis, _ui_menu_switch_set_value},
    {"z_operational_current", ui_setting_z_operational_current, _ui_menu_spinbox_set_value},
    {"z_idle_behavior", ui_setting_z_idle_behavior, _ui_menu_dropdown_set_value},
    {"z_sleep_current", ui_setting_z_sleep_current, _ui_menu_spinbox_set_value},

    {"power_voltage", ui_setting_power_voltage, _ui_menu_dropdown_set_value},

};


static const ui_setting_menu_mapping_t * __ui_menu_get_mapping(const char * item_name)
{
    uint16_t i;
    for(i = 0; setting_mapping[i].obj_name != NULL; i++) {
        if(strcmp(setting_mapping[i].obj_name, item_name) == 0) {
            /*The item_name has found. Check the translation*/
            return &setting_mapping[i];
        }
    }

    return NULL;
}


void ui_menu_on_val_change(puncher_event_setting_change_t * data)
{
    const ui_setting_menu_mapping_t *item = __ui_menu_get_mapping(data->item_name);
    if (item != NULL)
    {
        item->call_back(item->obj, data->data);
    }
}

#endif