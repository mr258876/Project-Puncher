// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.3
// LVGL version: 8.3.6
// Project name: Project_Puncher_NG

#ifndef _PROJECT_PUNCHER_NG_UI_H
#define _PROJECT_PUNCHER_NG_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
  #if __has_include("lvgl.h")
    #include "lvgl.h"
  #elif __has_include("lvgl/lvgl.h")
    #include "lvgl/lvgl.h"
  #else
    #include "lvgl.h"
  #endif
#else
  #include "lvgl.h"
#endif

#include "assets/ui_font_noto_sans.h"
#include "i18n/lv_i18n.h"
#include "ui_helpers.h"
#include "ui_events.h"
#include "ui_setting_menu.h"
// SCREEN: ui_HomeScreen
void ui_HomeScreen_screen_init(void);
extern lv_obj_t *ui_HomeScreen;
extern lv_obj_t *ui_InfoBar;
extern lv_obj_t *ui_InfoBarTitle1;
extern lv_obj_t *ui_InfoBarTitle2;
extern lv_obj_t *ui_StatusPanel;
extern lv_obj_t *ui_Label4;
extern lv_obj_t *ui_Label5;
extern lv_obj_t *ui_Label6;
extern lv_obj_t *ui_Label7;
extern lv_obj_t *ui_OperationPanel;
void ui_event_FeedButton( lv_event_t * e);
extern lv_obj_t *ui_FeedButton;
extern lv_obj_t *ui_FeedButtonLabel;
void ui_event_StartButton( lv_event_t * e);
extern lv_obj_t *ui_StartButton;
extern lv_obj_t *ui_StartButtonLabel;
void ui_event_PauseButton( lv_event_t * e);
extern lv_obj_t *ui_PauseButton;
extern lv_obj_t *ui_PauseButtonLabel;
void ui_event_StopButton( lv_event_t * e);
extern lv_obj_t *ui_StopButton;
extern lv_obj_t *ui_StopButtonLabel;
void ui_event_SettingButton( lv_event_t * e);
extern lv_obj_t *ui_SettingButton;
extern lv_obj_t *ui_SettingButtonLabel;
// SCREEN: ui_FeedScreen
void ui_FeedScreen_screen_init(void);
void ui_event_FeedScreen( lv_event_t * e);
extern lv_obj_t *ui_FeedScreen;
extern lv_obj_t *ui_FeedTopBar;
void ui_event_FeedBackButton( lv_event_t * e);
extern lv_obj_t *ui_FeedBackButton;
extern lv_obj_t *ui_FeedBackButtonLabel;
extern lv_obj_t *ui_FeedTitle;
extern lv_obj_t *ui_FeedPanel;
void ui_event_FeedStopButton( lv_event_t * e);
extern lv_obj_t *ui_FeedStopButton;
extern lv_obj_t *ui_FeedStopButtonLabel;
extern lv_obj_t *ui_FeedSpeedPanel;
void ui_event_FeedPlusButton( lv_event_t * e);
extern lv_obj_t *ui_FeedPlusButton;
extern lv_obj_t *ui_FeedPlusButtonLabel;
extern lv_obj_t *ui_FeedRoller;
void ui_event_FeedMinusButton( lv_event_t * e);
extern lv_obj_t *ui_FeedMinusButton;
extern lv_obj_t *ui_FeedMinusButtonLabel;
extern lv_obj_t *ui____initial_actions0;
// SCREEN: ui_SettingScreen
void ui_SettingScreen_screen_init(void);
extern lv_obj_t *ui_SettingScreen;
extern lv_obj_t *ui____initial_actions0;

extern lv_timer_t *ui_ETA_timer;


void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
