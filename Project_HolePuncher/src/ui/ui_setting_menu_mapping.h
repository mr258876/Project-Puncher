#ifndef _UI_SETTING_MENU_MAPPING_H_
#define _UI_SETTING_MENU_MAPPING_H_

#include "lvgl.h"
#include "ui_setting_menu.h"
#include "ui_setting_menu_helpers.h"
#include <any>
#include "esp_log.h"

struct ui_setting_menu_mapping_t
{
    const char *obj_name;
    lv_obj_t *obj;
    void (*call_back)(lv_obj_t *obj, std::any param);
};


ui_setting_menu_mapping_t setting_mapping[] = {
    {"wifi_switch", NULL, _ui_menu_switch_set_value},

    {"x_lead_length", NULL, _ui_menu_spinbox_set_value},
    {"x_operational_speed", NULL, _ui_menu_spinbox_set_value},
    {"x_reverse_axis", NULL, _ui_menu_switch_set_value},
    {"x_operational_current", NULL, _ui_menu_spinbox_set_value},
    {"x_idle_behavior", NULL, _ui_menu_dropdown_set_value},
    {"x_sleep_current", NULL, _ui_menu_spinbox_set_value},
    {"x_auto_zreoing", NULL, _ui_menu_switch_set_value},
    {"x_zeroing_torch_thres", NULL, _ui_menu_spinbox_set_value},
    {"x_zeroing_current", NULL, _ui_menu_spinbox_set_value},
    {"x_zeroing_speed", NULL, _ui_menu_spinbox_set_value},

    {"y_lead_length", NULL, _ui_menu_spinbox_set_value},
    {"y_operational_speed", NULL, _ui_menu_spinbox_set_value},
    {"y_reverse_axis", NULL, _ui_menu_switch_set_value},
    {"y_operational_current", NULL, _ui_menu_spinbox_set_value},
    {"y_idle_behavior", NULL, _ui_menu_dropdown_set_value},
    {"y_sleep_current", NULL, _ui_menu_spinbox_set_value},
    {"y_auto_zreoing", NULL, _ui_menu_switch_set_value},
    {"y_zeroing_torch_thres", NULL, _ui_menu_spinbox_set_value},
    {"y_zeroing_current", NULL, _ui_menu_spinbox_set_value},
    {"y_zeroing_speed", NULL, _ui_menu_spinbox_set_value},

    {"z_lead_length", NULL, _ui_menu_spinbox_set_value},
    {"z_operational_speed", NULL, _ui_menu_spinbox_set_value},
    {"z_reverse_axis", NULL, _ui_menu_switch_set_value},
    {"z_operational_current", NULL, _ui_menu_spinbox_set_value},
    {"z_idle_behavior", NULL, _ui_menu_dropdown_set_value},
    {"z_sleep_current", NULL, _ui_menu_spinbox_set_value},

    {"power_voltage", NULL, _ui_menu_dropdown_set_value},

    {"display_brightness", NULL, NULL},
    {"display_language", NULL, _ui_menu_set_language},
};


static ui_setting_menu_mapping_t * __ui_menu_get_mapping(const char * item_name)
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
    ui_setting_menu_mapping_t *item = __ui_menu_get_mapping(data->item_name);
    if (item != NULL)
    {   
        ESP_LOGI("ui_menu", "name: %s", item->obj_name);
        ESP_LOGI("ui_menu", "addr: %p", item->obj);
        ESP_LOGI("ui_menu", "changing value");
        item->call_back(item->obj, data->data);
        ESP_LOGI("ui_menu", "value changed");
    }
}

void ui_menu_ptr_update()
{
    setting_mapping[0].obj = ui_setting_wifi_switch;

    setting_mapping[1].obj = ui_setting_x_lead_length;
    setting_mapping[2].obj = ui_setting_x_operational_speed;
    setting_mapping[3].obj = ui_setting_x_reverse_axis;
    setting_mapping[4].obj = ui_setting_x_operational_current;
    setting_mapping[5].obj = ui_setting_x_idle_behavior;
    setting_mapping[6].obj = ui_setting_x_sleep_current;
    setting_mapping[7].obj = ui_setting_x_auto_zreoing;
    setting_mapping[8].obj = ui_setting_x_zeroing_torch_thres;
    setting_mapping[9].obj = ui_setting_x_zeroing_current;
    setting_mapping[10].obj = ui_setting_x_zeroing_speed;

    setting_mapping[11].obj = ui_setting_y_lead_length;
    setting_mapping[12].obj = ui_setting_y_operational_speed;
    setting_mapping[13].obj = ui_setting_y_reverse_axis;
    setting_mapping[14].obj = ui_setting_y_operational_current;
    setting_mapping[15].obj = ui_setting_y_idle_behavior;
    setting_mapping[16].obj = ui_setting_y_sleep_current;
    setting_mapping[17].obj = ui_setting_y_auto_zreoing;
    setting_mapping[18].obj = ui_setting_y_zeroing_torch_thres;
    setting_mapping[19].obj = ui_setting_y_zeroing_current;
    setting_mapping[20].obj = ui_setting_y_zeroing_speed;

    setting_mapping[21].obj = ui_setting_z_lead_length;
    setting_mapping[22].obj = ui_setting_z_operational_speed;
    setting_mapping[23].obj = ui_setting_z_reverse_axis;
    setting_mapping[24].obj = ui_setting_z_operational_current;
    setting_mapping[25].obj = ui_setting_z_idle_behavior;
    setting_mapping[26].obj = ui_setting_z_sleep_current;

    setting_mapping[27].obj = ui_setting_power_voltage;

    setting_mapping[28].obj = ui_setting_display_brightness;
    setting_mapping[29].obj = ui_setting_display_language;
}

#endif