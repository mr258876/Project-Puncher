#ifndef _UI_SETTING_MENU_MAPPING_H_
#define _UI_SETTING_MENU_MAPPING_H_

#include "lvgl.h"
#include "ui_setting_menu.h"

struct ui_setting_menu_mapping_t
{
    const char* obj_name;
    lv_obj_t *obj;
    void (* call_back)(void *param);
};

const ui_setting_menu_mapping_t setting_mapping[] = {
    {"wifi_switch", ui_setting_wifi_switch},
    
};


#endif