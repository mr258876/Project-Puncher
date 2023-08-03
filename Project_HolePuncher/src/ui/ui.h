// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.3.6
// Project name: Project_Puncher

#ifndef _PROJECT_PUNCHER_UI_H
#define _PROJECT_PUNCHER_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

#include "i18n/lv_i18n.h"
#include "ui_helpers.h"
#include "ui_events.h"
// SCREEN: ui_Home_Screen
void ui_Home_Screen_screen_init(void);
extern lv_obj_t * ui_Home_Screen;
extern lv_obj_t * ui_Home_Title_Panel;
extern lv_obj_t * ui_Home_Title;
extern lv_obj_t * ui_Status_Icons;
extern lv_obj_t * ui_Home_Content_Panel;
extern lv_obj_t * ui_Home_Status;
extern lv_obj_t * ui_Home_ETA;
extern lv_obj_t * ui_Home_ETA_Value;
extern lv_obj_t * ui_Home_Button_Panel;
void ui_event_Home_Button1(lv_event_t * e);
extern lv_obj_t * ui_Home_Button1;
extern lv_obj_t * ui_Home_Button1_Label;
void ui_event_Home_Button2(lv_event_t * e);
extern lv_obj_t * ui_Home_Button2;
extern lv_obj_t * ui_Home_Button2_Label;
// SCREEN: ui_Feed_Screen
void ui_Feed_Screen_screen_init(void);
extern lv_obj_t * ui_Feed_Screen;
extern lv_obj_t * ui_Feed_Title_Panel;
extern lv_obj_t * ui_Feed_title;
extern lv_obj_t * ui_Feed_Roller;
// SCREEN: ui_Info_Screen
void ui_Info_Screen_screen_init(void);
extern lv_obj_t * ui_Info_Screen;
extern lv_obj_t * ui_Info_Title_Panel;
extern lv_obj_t * ui_Info_title;
extern lv_obj_t * ui_Info_Content_Panel;
extern lv_obj_t * ui_Info_Text_Label;
extern lv_obj_t * ui_Info_Button_Panel;
void ui_event_Info_Button1(lv_event_t * e);
extern lv_obj_t * ui_Info_Button1;
extern lv_obj_t * ui_Info_Button1_Label;
void ui_event_Info_Button2(lv_event_t * e);
extern lv_obj_t * ui_Info_Button2;
extern lv_obj_t * ui_Info_Button2_Label;
// SCREEN: ui_Setting_Screen
void ui_Setting_Screen_screen_init(void);
extern lv_obj_t * ui_Setting_Screen;
extern lv_obj_t * ui_Setting_Menu;

extern lv_obj_t * ui____initial_actions0;

extern lv_group_t * ui_group;

void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif