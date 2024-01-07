#include "../ui.h"

static void ui_cb_ETA_timer(lv_timer_t *timer);

void ui_HomeScreen_screen_init(void)
{
    ui_HomeScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_HomeScreen, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_flex_flow(ui_HomeScreen, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_HomeScreen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_left(ui_HomeScreen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_HomeScreen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_HomeScreen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_HomeScreen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_HomeScreen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_HomeScreen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_InfoBar = lv_obj_create(ui_HomeScreen);
    lv_obj_set_width(ui_InfoBar, lv_pct(100));
    lv_obj_set_height(ui_InfoBar, lv_pct(20));
    lv_obj_set_align(ui_InfoBar, LV_ALIGN_TOP_MID);
    lv_obj_set_flex_flow(ui_InfoBar, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_InfoBar, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_clear_flag(ui_InfoBar, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_radius(ui_InfoBar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_InfoBarTitle1 = lv_label_create(ui_InfoBar);
    lv_obj_set_width(ui_InfoBarTitle1, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_InfoBarTitle1, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_InfoBarTitle1, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_InfoBarTitle1, _("Project Puncher"));
    lv_obj_set_style_text_font(ui_InfoBarTitle1, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_InfoBarTitle2 = lv_label_create(ui_InfoBar);
    lv_obj_set_width(ui_InfoBarTitle2, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_InfoBarTitle2, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_InfoBarTitle2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_InfoBarTitle2, _("Next Gen"));
    lv_obj_set_style_text_decor(ui_InfoBarTitle2, LV_TEXT_DECOR_UNDERLINE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_InfoBarTitle2, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_StatusPanel = lv_obj_create(ui_HomeScreen);
    lv_obj_set_width(ui_StatusPanel, lv_pct(75));
    lv_obj_set_height(ui_StatusPanel, lv_pct(80));
    lv_obj_set_align(ui_StatusPanel, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_StatusPanel, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_StatusPanel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_clear_flag(ui_StatusPanel, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_radius(ui_StatusPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label4 = lv_label_create(ui_StatusPanel);
    lv_obj_set_width(ui_Label4, lv_pct(100));
    lv_obj_set_height(ui_Label4, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_Label4, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label4, _("Status"));
    lv_obj_set_style_text_font(ui_Label4, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label5 = lv_label_create(ui_StatusPanel);
    lv_obj_set_width(ui_Label5, lv_pct(100));
    lv_obj_set_height(ui_Label5, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_Label5, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label5, "Idle");
    lv_obj_set_style_text_font(ui_Label5, &lv_font_montserrat_36, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label6 = lv_label_create(ui_StatusPanel);
    lv_obj_set_width(ui_Label6, lv_pct(100));
    lv_obj_set_height(ui_Label6, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_Label6, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label6, _("ETA"));
    lv_obj_set_style_text_font(ui_Label6, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label7 = lv_label_create(ui_StatusPanel);
    lv_obj_set_width(ui_Label7, lv_pct(100));
    lv_obj_set_height(ui_Label7, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_Label7, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label7, "88:88:88");
    lv_obj_set_style_text_font(ui_Label7, &lv_font_montserrat_36, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_OperationPanel = lv_obj_create(ui_HomeScreen);
    lv_obj_set_width(ui_OperationPanel, lv_pct(25));
    lv_obj_set_height(ui_OperationPanel, lv_pct(80));
    lv_obj_set_align(ui_OperationPanel, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_OperationPanel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_OperationPanel, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scroll_dir(ui_OperationPanel, LV_DIR_VER);
    lv_obj_set_style_radius(ui_OperationPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_FeedButton = lv_btn_create(ui_OperationPanel);
    lv_obj_set_width(ui_FeedButton, lv_pct(100));
    lv_obj_set_height(ui_FeedButton, lv_pct(30));
    lv_obj_set_x(ui_FeedButton, 193);
    lv_obj_set_y(ui_FeedButton, -117);
    lv_obj_set_align(ui_FeedButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_FeedButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_FeedButton, LV_OBJ_FLAG_SCROLLABLE);    /// Flags
    lv_obj_set_style_radius(ui_FeedButton, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_FeedButton, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_FeedButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_FeedButtonLabel = lv_label_create(ui_FeedButton);
    lv_obj_set_width(ui_FeedButtonLabel, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_FeedButtonLabel, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_FeedButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_FeedButtonLabel, LV_SYMBOL_DRIVE);

    ui_StartButton = lv_btn_create(ui_OperationPanel);
    lv_obj_set_width(ui_StartButton, lv_pct(100));
    lv_obj_set_height(ui_StartButton, lv_pct(30));
    lv_obj_set_x(ui_StartButton, 193);
    lv_obj_set_y(ui_StartButton, -117);
    lv_obj_set_align(ui_StartButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_StartButton, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_StartButton, LV_OBJ_FLAG_SCROLLABLE);                         /// Flags
    lv_obj_set_style_radius(ui_StartButton, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_StartButton, lv_color_hex(0x47855A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_StartButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_StartButtonLabel = lv_label_create(ui_StartButton);
    lv_obj_set_width(ui_StartButtonLabel, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_StartButtonLabel, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_StartButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_StartButtonLabel, LV_SYMBOL_PLAY);

    ui_PauseButton = lv_btn_create(ui_OperationPanel);
    lv_obj_set_width(ui_PauseButton, lv_pct(100));
    lv_obj_set_height(ui_PauseButton, lv_pct(30));
    lv_obj_set_x(ui_PauseButton, 193);
    lv_obj_set_y(ui_PauseButton, -117);
    lv_obj_set_align(ui_PauseButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_PauseButton, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_PauseButton, LV_OBJ_FLAG_SCROLLABLE);                         /// Flags
    lv_obj_set_style_radius(ui_PauseButton, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_PauseButton, lv_color_hex(0xE7BD39), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PauseButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PauseButtonLabel = lv_label_create(ui_PauseButton);
    lv_obj_set_width(ui_PauseButtonLabel, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_PauseButtonLabel, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_PauseButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_PauseButtonLabel, LV_SYMBOL_PAUSE);

    ui_StopButton = lv_btn_create(ui_OperationPanel);
    lv_obj_set_width(ui_StopButton, lv_pct(100));
    lv_obj_set_height(ui_StopButton, lv_pct(30));
    lv_obj_set_x(ui_StopButton, 193);
    lv_obj_set_y(ui_StopButton, -117);
    lv_obj_set_align(ui_StopButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_StopButton, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_StopButton, LV_OBJ_FLAG_SCROLLABLE);                         /// Flags
    lv_obj_set_style_radius(ui_StopButton, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_StopButton, lv_color_hex(0xAB4F3F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_StopButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_StopButtonLabel = lv_label_create(ui_StopButton);
    lv_obj_set_width(ui_StopButtonLabel, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_StopButtonLabel, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_StopButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_StopButtonLabel, LV_SYMBOL_STOP);

    ui_SettingButton = lv_btn_create(ui_OperationPanel);
    lv_obj_set_width(ui_SettingButton, lv_pct(100));
    lv_obj_set_height(ui_SettingButton, lv_pct(30));
    lv_obj_set_x(ui_SettingButton, 193);
    lv_obj_set_y(ui_SettingButton, -117);
    lv_obj_set_align(ui_SettingButton, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_SettingButton, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_SettingButton, LV_OBJ_FLAG_SCROLLABLE);    /// Flags
    lv_obj_set_style_radius(ui_SettingButton, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_SettingButton, lv_color_hex(0x384871), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_SettingButton, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SettingButtonLabel = lv_label_create(ui_SettingButton);
    lv_obj_set_width(ui_SettingButtonLabel, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_SettingButtonLabel, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_SettingButtonLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_SettingButtonLabel, LV_SYMBOL_SETTINGS);

    lv_obj_add_event_cb(ui_FeedButton, ui_event_FeedButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_StartButton, ui_event_StartButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PauseButton, ui_event_PauseButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_StopButton, ui_event_StopButton, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_SettingButton, ui_event_SettingButton, LV_EVENT_ALL, NULL);

    time_t *eta_user_data = lv_mem_alloc(sizeof(time_t));
    ui_ETA_timer = lv_timer_create(ui_cb_ETA_timer, 1000, eta_user_data);
    lv_timer_pause(ui_ETA_timer);
}

static void ui_cb_ETA_timer(lv_timer_t *timer)
{
    time_t *eta = (time_t *)timer->user_data;
    *eta -= 1;
    lv_label_set_text_fmt(ui_Label7, "%02ld:%02ld:%02ld", *eta / 3600, *eta % 3600 / 60, *eta % 60);
}