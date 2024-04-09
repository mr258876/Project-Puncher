#ifndef _UI_SETTING_MENU_HELPERS_H_
#define _UI_SETTING_MENU_HELPERS_H_

#include "lvgl.h"
#include <any>
#include <stdint.h>

void _ui_menu_switch_set_value(lv_obj_t *obj, std::any param, std::any data);
// void _ui_menu_roller_set_value(lv_obj_t *obj, std::any param, std::any data);
void _ui_menu_slider_set_value(lv_obj_t *obj, std::any param, std::any data);
void _ui_menu_dropdown_set_value(lv_obj_t *obj, std::any param, std::any data);
void _ui_menu_spinbox_set_value(lv_obj_t *obj, std::any param, std::any data);
void _ui_menu_txt_fmt_set_int32(lv_obj_t *obj, std::any param, std::any data);

void _ui_menu_set_brightness(lv_obj_t *obj, std::any param, std::any data);
void _ui_menu_set_language(lv_obj_t *obj, std::any param, std::any data);

#endif // _UI_SETTING_MENU_HELPERS_H_