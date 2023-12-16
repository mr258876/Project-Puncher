#ifndef _UI_SETTING_MENU_HELPERS_H_
#define _UI_SETTING_MENU_HELPERS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

void _ui_menu_switch_set_value(lv_obj_t *obj, void *param);
void _ui_menu_roller_set_value(lv_obj_t *obj, void *param);
void _ui_menu_dropdown_set_value(lv_obj_t *obj, void *param);
void _ui_menu_spinbox_set_value(lv_obj_t *obj, void *param);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // _UI_SETTING_MENU_HELPERS_H_