// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.3
// LVGL version: 8.3.6
// Project name: Project_Puncher_NG

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////

// SCREEN: ui_HomeScreen
void ui_HomeScreen_screen_init(void);
lv_obj_t *ui_HomeScreen;
lv_obj_t *ui_InfoBar;
lv_obj_t *ui_InfoBarTitle1;
lv_obj_t *ui_InfoBarTitle2;
lv_obj_t *ui_StatusPanel;
lv_obj_t *ui_Label4;
lv_obj_t *ui_Label5;
lv_obj_t *ui_Label6;
lv_obj_t *ui_Label7;
lv_obj_t *ui_OperationPanel;
void ui_event_FeedButton(lv_event_t *e);
lv_obj_t *ui_FeedButton;
lv_obj_t *ui_FeedButtonLabel;
lv_obj_t *ui_StartButton;
lv_obj_t *ui_StartButtonLabel;
lv_obj_t *ui_PauseButton;
lv_obj_t *ui_PauseButtonLabel;
lv_obj_t *ui_StopButton;
lv_obj_t *ui_StopButtonLabel;
void ui_event_SettingButton(lv_event_t *e);
lv_obj_t *ui_SettingButton;
lv_obj_t *ui_SettingButtonLabel;

// SCREEN: ui_FeedScreen
void ui_FeedScreen_screen_init(void);
void ui_event_FeedScreen(lv_event_t *e);
lv_obj_t *ui_FeedScreen;
lv_obj_t *ui_FeedTopBar;
void ui_event_FeedBackButton(lv_event_t *e);
lv_obj_t *ui_FeedBackButton;
lv_obj_t *ui_FeedBackButtonLabel;
lv_obj_t *ui_FeedTitle;
lv_obj_t *ui_FeedPanel;
void ui_event_FeedStopButton(lv_event_t *e);
lv_obj_t *ui_FeedStopButton;
lv_obj_t *ui_FeedStopButtonLabel;
lv_obj_t *ui_FeedSpeedPanel;
void ui_event_FeedPlusButton(lv_event_t *e);
lv_obj_t *ui_FeedPlusButton;
lv_obj_t *ui_FeedPlusButtonLabel;
lv_obj_t *ui_FeedRoller;
void ui_event_FeedMinusButton(lv_event_t *e);
lv_obj_t *ui_FeedMinusButton;
lv_obj_t *ui_FeedMinusButtonLabel;
lv_obj_t *ui____initial_actions0;

// SCREEN: ui_SettingScreen
void ui_SettingScreen_screen_init(void);
lv_obj_t *ui_SettingScreen;
lv_obj_t *ui____initial_actions0;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
#error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP != 1
#error "LV_COLOR_16_SWAP should be 1 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_FeedButton(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        _ui_screen_change(&ui_FeedScreen, LV_SCR_LOAD_ANIM_FADE_ON, 250, 0, &ui_FeedScreen_screen_init);
    }
}
void ui_event_SettingButton(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        _ui_screen_change(&ui_SettingScreen, LV_SCR_LOAD_ANIM_FADE_ON, 250, 0, &ui_SettingScreen_screen_init);
    }
}
void ui_event_FeedScreen(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_SCREEN_LOAD_START)
    {
        _ui_roller_set_property(ui_FeedRoller, _UI_ROLLER_PROPERTY_SELECTED, 4);
    }
}
void ui_event_FeedBackButton(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        _ui_screen_change(&ui_HomeScreen, LV_SCR_LOAD_ANIM_FADE_ON, 250, 0, &ui_HomeScreen_screen_init);
    }
}
void ui_event_FeedStopButton(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        _ui_roller_set_property(ui_FeedRoller, _UI_ROLLER_PROPERTY_SELECTED_WITH_ANIM, 4);
    }
}
void ui_event_FeedPlusButton(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        _ui_roller_set_property(ui_FeedRoller, _UI_ROLLER_PROPERTY_SELECTED_WITH_ANIM, (lv_roller_get_selected(ui_FeedRoller) <= 0 ? 0 : (lv_roller_get_selected(ui_FeedRoller) - 1)));
    }
}
void ui_event_FeedMinusButton(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        _ui_roller_set_property(ui_FeedRoller, _UI_ROLLER_PROPERTY_SELECTED_WITH_ANIM, (lv_roller_get_selected(ui_FeedRoller) >= 8 ? 8 : (lv_roller_get_selected(ui_FeedRoller) + 1)));
    }
}

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_i18n_init(lv_i18n_language_pack);

    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp,                                                   /*Use the DPI, size, etc from this display*/
                                              lv_color_make(94, 166, 156), lv_color_make(156, 49, 41), /*Primary and secondary palette*/
                                              false,                                                   /*Light or dark mode*/
                                              LV_FONT_DEFAULT);                                        /*Small, normal, large fonts*/
    lv_disp_set_theme(dispp, theme);
    ui_HomeScreen_screen_init();
    ui_FeedScreen_screen_init();
    ui_SettingScreen_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_HomeScreen);
}
