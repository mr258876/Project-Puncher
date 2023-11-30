// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.3
// LVGL version: 8.3.6
// Project name: Project_Puncher_NG

#include "../ui.h"

void ui_HomeScreen_screen_init(void)
{
ui_HomeScreen = lv_obj_create(NULL);
lv_obj_clear_flag( ui_HomeScreen, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_flex_flow(ui_HomeScreen,LV_FLEX_FLOW_ROW_WRAP);
lv_obj_set_flex_align(ui_HomeScreen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
lv_obj_set_style_pad_left(ui_HomeScreen, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_HomeScreen, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_HomeScreen, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_HomeScreen, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_row(ui_HomeScreen, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_column(ui_HomeScreen, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Panel1 = lv_obj_create(ui_HomeScreen);
lv_obj_set_width( ui_Panel1, lv_pct(100));
lv_obj_set_height( ui_Panel1, lv_pct(20));
lv_obj_set_align( ui_Panel1, LV_ALIGN_TOP_MID );
lv_obj_set_flex_flow(ui_Panel1,LV_FLEX_FLOW_ROW_WRAP);
lv_obj_set_flex_align(ui_Panel1, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
lv_obj_clear_flag( ui_Panel1, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_Panel1, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label1 = lv_label_create(ui_Panel1);
lv_obj_set_width( ui_Label1, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label1, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label1, LV_ALIGN_LEFT_MID );
lv_label_set_text(ui_Label1,_("Project Puncher"));
lv_obj_set_style_text_font(ui_Label1, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label2 = lv_label_create(ui_Panel1);
lv_obj_set_width( ui_Label2, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label2, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label2, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label2,_("Next Gen"));
lv_obj_set_style_text_decor(ui_Label2, LV_TEXT_DECOR_UNDERLINE, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_Label2, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Panel2 = lv_obj_create(ui_HomeScreen);
lv_obj_set_width( ui_Panel2, lv_pct(75));
lv_obj_set_height( ui_Panel2, lv_pct(80));
lv_obj_set_align( ui_Panel2, LV_ALIGN_CENTER );
lv_obj_set_flex_flow(ui_Panel2,LV_FLEX_FLOW_ROW_WRAP);
lv_obj_set_flex_align(ui_Panel2, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
lv_obj_clear_flag( ui_Panel2, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_Panel2, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label4 = lv_label_create(ui_Panel2);
lv_obj_set_width( ui_Label4, lv_pct(100));
lv_obj_set_height( ui_Label4, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label4, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label4,_("Status"));
lv_obj_set_style_text_font(ui_Label4, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label5 = lv_label_create(ui_Panel2);
lv_obj_set_width( ui_Label5, lv_pct(100));
lv_obj_set_height( ui_Label5, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label5, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label5,"Idle");
lv_obj_set_style_text_font(ui_Label5, &lv_font_montserrat_36, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label6 = lv_label_create(ui_Panel2);
lv_obj_set_width( ui_Label6, lv_pct(100));
lv_obj_set_height( ui_Label6, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label6, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label6,_("ETA"));
lv_obj_set_style_text_font(ui_Label6, &lv_font_montserrat_16, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label7 = lv_label_create(ui_Panel2);
lv_obj_set_width( ui_Label7, lv_pct(100));
lv_obj_set_height( ui_Label7, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label7, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label7,"88:88:88");
lv_obj_set_style_text_font(ui_Label7, &lv_font_montserrat_36, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Panel3 = lv_obj_create(ui_HomeScreen);
lv_obj_set_width( ui_Panel3, lv_pct(25));
lv_obj_set_height( ui_Panel3, lv_pct(80));
lv_obj_set_align( ui_Panel3, LV_ALIGN_CENTER );
lv_obj_set_flex_flow(ui_Panel3,LV_FLEX_FLOW_COLUMN);
lv_obj_set_flex_align(ui_Panel3, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
lv_obj_set_scroll_dir(ui_Panel3, LV_DIR_VER);
lv_obj_set_style_radius(ui_Panel3, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Button1 = lv_btn_create(ui_Panel3);
lv_obj_set_width( ui_Button1, lv_pct(100));
lv_obj_set_height( ui_Button1, lv_pct(30));
lv_obj_set_x( ui_Button1, 193 );
lv_obj_set_y( ui_Button1, -117 );
lv_obj_set_align( ui_Button1, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Button1, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_Button1, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_Button1, 25, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label3 = lv_label_create(ui_Button1);
lv_obj_set_width( ui_Label3, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label3, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label3, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label3,_("Feed"));

ui_Button2 = lv_btn_create(ui_Panel3);
lv_obj_set_width( ui_Button2, lv_pct(100));
lv_obj_set_height( ui_Button2, lv_pct(30));
lv_obj_set_x( ui_Button2, 193 );
lv_obj_set_y( ui_Button2, -117 );
lv_obj_set_align( ui_Button2, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Button2, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_Button2, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_Button2, 25, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label9 = lv_label_create(ui_Button2);
lv_obj_set_width( ui_Label9, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label9, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label9, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label9,_("Pause_SIGN"));

ui_Button3 = lv_btn_create(ui_Panel3);
lv_obj_set_width( ui_Button3, lv_pct(100));
lv_obj_set_height( ui_Button3, lv_pct(30));
lv_obj_set_x( ui_Button3, 193 );
lv_obj_set_y( ui_Button3, -117 );
lv_obj_set_align( ui_Button3, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Button3, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_Button3, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_Button3, 25, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label10 = lv_label_create(ui_Button3);
lv_obj_set_width( ui_Label10, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label10, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label10, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label10,_("Continue_SIGN"));

ui_Button4 = lv_btn_create(ui_Panel3);
lv_obj_set_width( ui_Button4, lv_pct(100));
lv_obj_set_height( ui_Button4, lv_pct(30));
lv_obj_set_x( ui_Button4, 193 );
lv_obj_set_y( ui_Button4, -117 );
lv_obj_set_align( ui_Button4, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Button4, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_Button4, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_Button4, 25, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label11 = lv_label_create(ui_Button4);
lv_obj_set_width( ui_Label11, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label11, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label11, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label11,_("Stop_SIGN"));

ui_Button7 = lv_btn_create(ui_Panel3);
lv_obj_set_width( ui_Button7, lv_pct(100));
lv_obj_set_height( ui_Button7, lv_pct(30));
lv_obj_set_x( ui_Button7, 193 );
lv_obj_set_y( ui_Button7, -117 );
lv_obj_set_align( ui_Button7, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_Button7, LV_OBJ_FLAG_SCROLL_ON_FOCUS );   /// Flags
lv_obj_clear_flag( ui_Button7, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_Button7, 25, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Label8 = lv_label_create(ui_Button7);
lv_obj_set_width( ui_Label8, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_Label8, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_Label8, LV_ALIGN_CENTER );
lv_label_set_text(ui_Label8,_("Settings_SIGN"));

}