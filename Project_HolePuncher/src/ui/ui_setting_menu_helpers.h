#ifndef _UI_SETTING_MENU_HELPERS_H_
#define _UI_SETTING_MENU_HELPERS_H_

#include "lvgl.h"
#include <any>
#include <stdint.h>

void _ui_menu_switch_set_value(lv_obj_t *obj, std::any param);
void _ui_menu_roller_set_value(lv_obj_t *obj, std::any param);
void _ui_menu_dropdown_set_value(lv_obj_t *obj, std::any param);
void _ui_menu_spinbox_set_value(lv_obj_t *obj, std::any param);


#endif // _UI_SETTING_MENU_HELPERS_H_