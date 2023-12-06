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
lv_obj_t *ui_Panel1;
lv_obj_t *ui_Label1;
lv_obj_t *ui_Label2;
lv_obj_t *ui_Panel2;
lv_obj_t *ui_Label4;
lv_obj_t *ui_Label5;
lv_obj_t *ui_Label6;
lv_obj_t *ui_Label7;
lv_obj_t *ui_Panel3;
void ui_event_Button1( lv_event_t * e);
lv_obj_t *ui_Button1;
lv_obj_t *ui_Label3;
lv_obj_t *ui_Button2;
lv_obj_t *ui_Label9;
lv_obj_t *ui_Button3;
lv_obj_t *ui_Label10;
lv_obj_t *ui_Button4;
lv_obj_t *ui_Label11;
lv_obj_t *ui_Button7;
lv_obj_t *ui_Label8;


// SCREEN: ui_FeedScreen
void ui_FeedScreen_screen_init(void);
void ui_event_FeedScreen( lv_event_t * e);
lv_obj_t *ui_FeedScreen;
lv_obj_t *ui_Panel5;
void ui_event_Button5( lv_event_t * e);
lv_obj_t *ui_Button5;
lv_obj_t *ui_Label12;
lv_obj_t *ui_Label13;
lv_obj_t *ui_Panel6;
lv_obj_t *ui_Panel4;
lv_obj_t *ui_Button6;
lv_obj_t *ui_Label14;
lv_obj_t *ui_Panel7;
void ui_event_Button8( lv_event_t * e);
lv_obj_t *ui_Button8;
lv_obj_t *ui_Label15;
lv_obj_t *ui_Roller1;
void ui_event_Button9( lv_event_t * e);
lv_obj_t *ui_Button9;
lv_obj_t *ui_Label16;
lv_obj_t *ui____initial_actions0;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=1
    #error "LV_COLOR_16_SWAP should be 1 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_Button1( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      _ui_screen_change( &ui_FeedScreen, LV_SCR_LOAD_ANIM_FADE_ON, 250, 0, &ui_FeedScreen_screen_init);
}
}
void ui_event_FeedScreen( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_SCREEN_LOAD_START) {
      _ui_roller_set_property(ui_Roller1, _UI_ROLLER_PROPERTY_SELECTED, 4);
}
}
void ui_event_Button5( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      _ui_screen_change( &ui_HomeScreen, LV_SCR_LOAD_ANIM_FADE_ON, 250, 0, &ui_HomeScreen_screen_init);
}
}
void ui_event_Button8( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      _ui_roller_set_property(ui_Roller1, _UI_ROLLER_PROPERTY_SELECTED_WITH_ANIM, (lv_roller_get_selected(ui_Roller1) <= 0 ? 0 : (lv_roller_get_selected(ui_Roller1) - 1)));
}
}
void ui_event_Button9( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      _ui_roller_set_property(ui_Roller1, _UI_ROLLER_PROPERTY_SELECTED_WITH_ANIM, (lv_roller_get_selected(ui_Roller1) >= 8 ? 8 : (lv_roller_get_selected(ui_Roller1) + 1)));
}
}

///////////////////// SCREENS ////////////////////

void ui_init( void )
{
lv_disp_t *dispp = lv_disp_get_default();
lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
lv_disp_set_theme(dispp, theme);
ui_HomeScreen_screen_init();
ui_FeedScreen_screen_init();
ui____initial_actions0 = lv_obj_create(NULL);
lv_disp_load_scr( ui_HomeScreen);
}
