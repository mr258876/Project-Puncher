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

///////////////////// TEST LVGL SETTINGS ////////////////////
// #if LV_COLOR_DEPTH != 8
//     #error "LV_COLOR_DEPTH should be 8bit to match SquareLine Studio's settings"
// #endif
// #if LV_COLOR_16_SWAP !=0
//     #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
// #endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_basic_init(dispp);
    lv_disp_set_theme(dispp, theme);
    ui_Home_Screen_screen_init();
    ui_Feed_Screen_screen_init();
    ui_Info_Screen_screen_init();
    ui_Setting_Screen_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_Home_Screen);
}
