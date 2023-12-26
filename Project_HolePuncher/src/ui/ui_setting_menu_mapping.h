#ifndef _UI_SETTING_MENU_MAPPING_H_
#define _UI_SETTING_MENU_MAPPING_H_

#include "lvgl.h"
#include "ui_setting_menu.h"
#include "ui_setting_menu_helpers.h"
#include <any>
#include <string>
#include <functional>
#include <unordered_map>
#include "esp_log.h"

struct ui_setting_menu_mapping_t
{
    ui_setting_menu_mapping_t(lv_obj_t *o, std::function<void(lv_obj_t *, std::any)> cb) : obj(o), call_back(cb) {}

    lv_obj_t *obj;
    std::function<void(lv_obj_t *, std::any)> call_back;
};

static std::unordered_map<std::string, ui_setting_menu_mapping_t> ui_setting_mapping;

void ui_menu_on_val_change(puncher_event_setting_change_t *data)
{
    auto it = ui_setting_mapping.find(data->item_name);
    if (it == ui_setting_mapping.end())
    {
        ESP_LOGD("ui_menu", "name: %s does not exist!", data->item_name);
    }
    else
    {
        ESP_LOGD("ui_menu", "changing value: name: %s, addr: %p", data->item_name, it->second.obj);
        it->second.call_back(it->second.obj, data->data);
        ESP_LOGD("ui_menu", "value changed");
    }
}

void ui_menu_ptr_update()
{
    ui_setting_mapping.emplace("wifi_switch", ui_setting_menu_mapping_t(ui_setting_wifi_switch, _ui_menu_switch_set_value));

    ui_setting_mapping.emplace("x_lead_length", ui_setting_menu_mapping_t(ui_setting_x_lead_length, _ui_menu_spinbox_set_value));
    ui_setting_mapping.emplace("x_operational_speed", ui_setting_menu_mapping_t(ui_setting_x_operational_speed, _ui_menu_spinbox_set_value));
    ui_setting_mapping.emplace("x_length_type", ui_setting_menu_mapping_t(ui_setting_x_length_type, _ui_menu_dropdown_set_value));
    ui_setting_mapping.emplace("x_reverse_axis", ui_setting_menu_mapping_t(ui_setting_x_reverse_axis, _ui_menu_switch_set_value));
    ui_setting_mapping.emplace("x_operational_current", ui_setting_menu_mapping_t(ui_setting_x_operational_current, _ui_menu_spinbox_set_value));
    ui_setting_mapping.emplace("x_idle_behavior", ui_setting_menu_mapping_t(ui_setting_x_idle_behavior, _ui_menu_dropdown_set_value));
    ui_setting_mapping.emplace("x_sleep_current", ui_setting_menu_mapping_t(ui_setting_x_sleep_current, _ui_menu_spinbox_set_value));
    ui_setting_mapping.emplace("x_auto_zreoing", ui_setting_menu_mapping_t(ui_setting_x_auto_zreoing, _ui_menu_switch_set_value));
    ui_setting_mapping.emplace("x_zeroing_torch_thres", ui_setting_menu_mapping_t(ui_setting_x_zeroing_torch_thres, _ui_menu_spinbox_set_value));
    ui_setting_mapping.emplace("x_zeroing_current", ui_setting_menu_mapping_t(ui_setting_x_zeroing_current, _ui_menu_spinbox_set_value));
    ui_setting_mapping.emplace("x_zeroing_speed", ui_setting_menu_mapping_t(ui_setting_x_zeroing_speed, _ui_menu_spinbox_set_value));

    ui_setting_mapping.emplace("y_lead_length", ui_setting_menu_mapping_t(ui_setting_y_lead_length, _ui_menu_spinbox_set_value));
    ui_setting_mapping.emplace("y_operational_speed", ui_setting_menu_mapping_t(ui_setting_y_operational_speed, _ui_menu_spinbox_set_value));
    ui_setting_mapping.emplace("y_length_type", ui_setting_menu_mapping_t(ui_setting_y_length_type, _ui_menu_dropdown_set_value));
    ui_setting_mapping.emplace("y_reverse_axis", ui_setting_menu_mapping_t(ui_setting_y_reverse_axis, _ui_menu_switch_set_value));
    ui_setting_mapping.emplace("y_operational_current", ui_setting_menu_mapping_t(ui_setting_y_operational_current, _ui_menu_spinbox_set_value));
    ui_setting_mapping.emplace("y_idle_behavior", ui_setting_menu_mapping_t(ui_setting_y_idle_behavior, _ui_menu_dropdown_set_value));
    ui_setting_mapping.emplace("y_sleep_current", ui_setting_menu_mapping_t(ui_setting_y_sleep_current, _ui_menu_spinbox_set_value));
    ui_setting_mapping.emplace("y_auto_zreoing", ui_setting_menu_mapping_t(ui_setting_y_auto_zreoing, _ui_menu_switch_set_value));
    ui_setting_mapping.emplace("y_zeroing_torch_thres", ui_setting_menu_mapping_t(ui_setting_y_zeroing_torch_thres, _ui_menu_spinbox_set_value));
    ui_setting_mapping.emplace("y_zeroing_current", ui_setting_menu_mapping_t(ui_setting_y_zeroing_current, _ui_menu_spinbox_set_value));
    ui_setting_mapping.emplace("y_zeroing_speed", ui_setting_menu_mapping_t(ui_setting_y_zeroing_speed, _ui_menu_spinbox_set_value));

    ui_setting_mapping.emplace("z_lead_length", ui_setting_menu_mapping_t(ui_setting_z_lead_length, _ui_menu_spinbox_set_value));
    ui_setting_mapping.emplace("z_operational_speed", ui_setting_menu_mapping_t(ui_setting_z_operational_speed, _ui_menu_spinbox_set_value));
    ui_setting_mapping.emplace("z_length_type", ui_setting_menu_mapping_t(ui_setting_z_length_type, _ui_menu_dropdown_set_value));
    ui_setting_mapping.emplace("z_reverse_axis", ui_setting_menu_mapping_t(ui_setting_z_reverse_axis, _ui_menu_switch_set_value));
    ui_setting_mapping.emplace("z_operational_current", ui_setting_menu_mapping_t(ui_setting_z_operational_current, _ui_menu_spinbox_set_value));
    ui_setting_mapping.emplace("z_idle_behavior", ui_setting_menu_mapping_t(ui_setting_z_idle_behavior, _ui_menu_dropdown_set_value));
    ui_setting_mapping.emplace("z_sleep_current", ui_setting_menu_mapping_t(ui_setting_z_sleep_current, _ui_menu_spinbox_set_value));
    ui_setting_mapping.emplace("z_cali_target_bar", ui_setting_menu_mapping_t(ui_setting_z_cali_target, _ui_menu_spinbox_set_value));
    ui_setting_mapping.emplace("z_cali_measure_bar", ui_setting_menu_mapping_t(ui_setting_z_cali_measure, _ui_menu_spinbox_set_value));
    ui_setting_mapping.emplace("z_cali_residual", ui_setting_menu_mapping_t(ui_setting_z_cali_residual, _ui_menu_spinbox_set_value));

    ui_setting_mapping.emplace("power_voltage", ui_setting_menu_mapping_t(ui_setting_power_voltage, _ui_menu_dropdown_set_value));

    ui_setting_mapping.emplace("display_brightness", ui_setting_menu_mapping_t(ui_setting_display_brightness, _ui_menu_set_brightness));
    ui_setting_mapping.emplace("display_language", ui_setting_menu_mapping_t(ui_setting_display_language, _ui_menu_set_language));

    ui_setting_mapping.emplace("mcode_default_tick_rate", ui_setting_menu_mapping_t(ui_setting_usb_mcode_tick_rate, _ui_menu_spinbox_set_value));
}

#endif