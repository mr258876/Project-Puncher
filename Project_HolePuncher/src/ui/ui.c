// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.3.6
// Project name: Project_Puncher

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////

// SCREEN: ui_Home_Screen
void ui_Home_Screen_screen_init(void);
lv_obj_t * ui_Home_Screen;
lv_obj_t * ui_Home_Title_Panel;
lv_obj_t * ui_Home_Title;
lv_obj_t * ui_Status_Icons;
lv_obj_t * ui_Home_Content_Panel;
lv_obj_t * ui_Home_Status;
lv_obj_t * ui_Home_ETA;
lv_obj_t * ui_Home_ETA_Value;
lv_obj_t * ui_Home_Button_Panel;
lv_obj_t * ui_Home_Button1;
lv_obj_t * ui_Home_Button1_Label;
lv_obj_t * ui_Home_Button2;
lv_obj_t * ui_Home_Button2_Label;

// SCREEN: ui_Feed_Screen
void ui_Feed_Screen_screen_init(void);
void ui_event_Feed_Screen(lv_event_t * e);
void ui_event_Feed_Roller(lv_event_t * e);
lv_obj_t * ui_Feed_Screen;
lv_obj_t * ui_Feed_Title_Panel;
lv_obj_t * ui_Feed_title;
lv_obj_t * ui_Feed_Roller;

// SCREEN: ui_Info_Screen
void ui_Info_Screen_screen_init(void);
lv_obj_t * ui_Info_Screen;
lv_obj_t * ui_Info_Title_Panel;
lv_obj_t * ui_Info_title;
lv_obj_t * ui_Info_Content_Panel;
lv_obj_t * ui_Info_Text_Label;
lv_obj_t * ui_Info_Button_Panel;
lv_obj_t * ui_Info_Button1;
lv_obj_t * ui_Info_Button1_Label;
lv_obj_t * ui_Info_Button2;
lv_obj_t * ui_Info_Button2_Label;

// SCREEN: ui_Setting_Screen
void ui_Setting_Screen_screen_init(void);
lv_obj_t * ui_Setting_Screen;
lv_obj_t * ui_Setting_Menu;

lv_obj_t * ui____initial_actions0;

lv_group_t * ui_group;

///////////////////// TEST LVGL SETTINGS ////////////////////
// #if LV_COLOR_DEPTH != 8
//     #error "LV_COLOR_DEPTH should be 8bit to match SquareLine Studio's settings"
// #endif
// #if LV_COLOR_16_SWAP !=0
//     #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
// #endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_Home_Button1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        // ui_event_Home_Button1_onClick(e);
        _ui_screen_change(&ui_Feed_Screen, LV_SCR_LOAD_ANIM_NONE, 20, 0, &ui_Feed_Screen_screen_init);
    }
}
void ui_event_Home_Button2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        // ui_event_Home_Button2_onClick(e);
        _ui_screen_change(&ui_Setting_Screen, LV_SCR_LOAD_ANIM_NONE, 20, 0, &ui_Setting_Screen_screen_init);
    }
}
void ui_event_Feed_Screen(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        ui_event_Feed_Screen_onLoad(e);
    }
    else if (event_code == LV_EVENT_SCREEN_UNLOADED)
    {
        lv_group_focus_freeze(ui_group, false);
        lv_group_set_editing(ui_group, false);
    }
    else if (event_code == LV_EVENT_DELETE)
    {
        ui_Feed_Screen = NULL;
    }
}
void ui_event_Feed_Roller(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_LONG_PRESSED || event_code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_scr_load_anim(ui_Home_Screen, LV_SCR_LOAD_ANIM_NONE, 20, 0, true);
    }
    else if (event_code == LV_EVENT_KEY)
    {
        ui_event_Feed_Roller_onKey(e);
    }
    
}
void ui_event_Info_Button1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        ui_event_Info_Button1_onClick(e);
    }
}
void ui_event_Info_Button2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        ui_event_Info_Button2_onClick(e);
    }
}

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_basic_init(dispp);

    lv_i18n_init(lv_i18n_language_pack);

    lv_disp_set_theme(dispp, theme);
    ui_Home_Screen_screen_init();
    // ui_Feed_Screen_screen_init();
    // ui_Info_Screen_screen_init();
    // ui_Setting_Screen_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_Home_Screen);
}
