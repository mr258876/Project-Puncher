#include "../ui.h"
#include "../ui_events.h"

lv_obj_t * ui_setting_root_page;
lv_obj_t * ui_setting_wifi_page;
lv_obj_t * ui_setting_ble_page;
lv_obj_t * ui_setting_usb_page;
lv_obj_t * ui_setting_ble_page;
lv_obj_t * ui_setting_usb_page;
lv_obj_t * ui_setting_x_axis_page;
lv_obj_t * ui_setting_y_axis_page;
lv_obj_t * ui_setting_z_axis_page;
lv_obj_t * ui_setting_power_page;
lv_obj_t * ui_setting_display_page;
lv_obj_t * ui_setting_about_page;

lv_obj_t * ui_setting_wifi_switch;

lv_obj_t * ui_setting_usb_mcode_tick_rate;

lv_obj_t * ui_setting_x_lead_length;
lv_obj_t * ui_setting_x_operational_speed;
lv_obj_t * ui_setting_x_length_type;
lv_obj_t * ui_setting_x_reverse_axis;
lv_obj_t * ui_setting_x_operational_current;
lv_obj_t * ui_setting_x_idle_behavior;
lv_obj_t * ui_setting_x_sleep_current;
lv_obj_t * ui_setting_x_auto_zreoing;
lv_obj_t * ui_setting_x_zreoing_reverse_dir;
lv_obj_t * ui_setting_x_zeroing_torch_thres;
lv_obj_t * ui_setting_x_zeroing_current;
lv_obj_t * ui_setting_x_zeroing_speed;
lv_obj_t * ui_setting_x_zeroing_position;
lv_obj_t * ui_setting_x_zeroing_start;
lv_obj_t * ui_setting_x_util_move_left;
lv_obj_t * ui_setting_x_util_move_right;
lv_obj_t * ui_setting_x_util_sg_result;
lv_obj_t * ui_setting_x_util_speed_type;

lv_obj_t * ui_setting_y_lead_length;
lv_obj_t * ui_setting_y_operational_speed;
lv_obj_t * ui_setting_y_length_type;
lv_obj_t * ui_setting_y_reverse_axis;
lv_obj_t * ui_setting_y_punch_depth;
lv_obj_t * ui_setting_y_operational_current;
lv_obj_t * ui_setting_y_idle_behavior;
lv_obj_t * ui_setting_y_sleep_current;
lv_obj_t * ui_setting_y_auto_zreoing;
lv_obj_t * ui_setting_y_zreoing_reverse_dir;
lv_obj_t * ui_setting_y_zeroing_torch_thres;
lv_obj_t * ui_setting_y_zeroing_current;
lv_obj_t * ui_setting_y_zeroing_speed;
lv_obj_t * ui_setting_y_zeroing_position;
lv_obj_t * ui_setting_y_zeroing_start;
lv_obj_t * ui_setting_y_util_move_up;
lv_obj_t * ui_setting_y_util_move_down;
lv_obj_t * ui_setting_y_util_sg_result;
lv_obj_t * ui_setting_y_util_speed_type;

lv_obj_t * ui_setting_z_lead_length;
lv_obj_t * ui_setting_z_operational_speed;
lv_obj_t * ui_setting_z_length_type;
lv_obj_t * ui_setting_z_reverse_axis;
lv_obj_t * ui_setting_z_operational_current;
lv_obj_t * ui_setting_z_idle_behavior;
lv_obj_t * ui_setting_z_sleep_current;
lv_obj_t * ui_setting_z_cali_target;
lv_obj_t * ui_setting_z_cali_measure;
lv_obj_t * ui_setting_z_cali_residual;
lv_obj_t * ui_setting_z_encoder_switch;
lv_obj_t * ui_setting_z_encoder_type;

lv_obj_t * ui_setting_power_voltage;

lv_obj_t * ui_setting_display_brightness;
lv_obj_t * ui_setting_display_language;

enum
{
    LV_MENU_ITEM_BUILDER_VARIANT_1,
    LV_MENU_ITEM_BUILDER_VARIANT_2
};
typedef uint8_t lv_menu_builder_variant_t;

static void back_event_handler(lv_event_t *e);
lv_obj_t *root_page;
static lv_obj_t *create_text(lv_obj_t *parent, const char *icon, const char *txt,
                             lv_menu_builder_variant_t builder_variant);
static lv_obj_t *create_slider(lv_obj_t *parent,
                               const char *icon, const char *txt, int32_t min, int32_t max, int32_t val, lv_event_cb_t event_cb);
static lv_obj_t *create_spinbox(lv_obj_t *parent, const char *icon, const char *txt, int32_t min, int32_t max,
                                int32_t val, int digit_count, int separator_position, lv_event_cb_t event_cb);
static lv_obj_t *create_switch(lv_obj_t *parent,
                               const char *icon, const char *txt, bool chk, lv_event_cb_t event_cb);

static void back_event_handler(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    lv_obj_t *menu = lv_event_get_user_data(e);

    if (lv_menu_back_btn_is_root(menu, obj))
    {
        _ui_screen_change(&ui_HomeScreen, LV_SCR_LOAD_ANIM_FADE_ON, 250, 0, &ui_HomeScreen_screen_init);
    }
}

static void lv_spinbox_increment_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        lv_spinbox_increment(e->user_data);
    }
}

static void lv_spinbox_decrement_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        lv_spinbox_decrement(e->user_data);
    }
}

static lv_obj_t *create_text(lv_obj_t *parent, const char *icon, const char *txt,
                             lv_menu_builder_variant_t builder_variant)
{
    lv_obj_t *obj = lv_menu_cont_create(parent);

    lv_obj_t *img = NULL;
    lv_obj_t *label = NULL;

    if (icon)
    {
        img = lv_img_create(obj);
        lv_img_set_src(img, icon);
    }

    if (txt)
    {
        label = lv_label_create(obj);
        lv_label_set_text(label, txt);
        lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_flex_grow(label, 1);
    }

    if (builder_variant == LV_MENU_ITEM_BUILDER_VARIANT_2 && icon && txt)
    {
        lv_obj_add_flag(img, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
        lv_obj_swap(img, label);
    }

    return obj;
}

static lv_obj_t *create_slider(lv_obj_t *parent, const char *icon, const char *txt, int32_t min, int32_t max,
                               int32_t val, lv_event_cb_t event_cb)
{
    lv_obj_t *obj = create_text(parent, icon, txt, LV_MENU_ITEM_BUILDER_VARIANT_2);

    lv_obj_t *slider = lv_slider_create(obj);
    lv_obj_set_flex_grow(slider, 1);
    lv_slider_set_range(slider, min, max);
    lv_slider_set_value(slider, val, LV_ANIM_OFF);

    if (event_cb != NULL)
    {
        lv_obj_add_event_cb(slider, event_cb, LV_EVENT_ALL, NULL);
    }

    if (icon == NULL)
    {
        lv_obj_add_flag(slider, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    }

    return obj;
}

static lv_obj_t *create_spinbox(lv_obj_t *parent, const char *icon, const char *txt, int32_t min, int32_t max,
                                int32_t val, int digit_count, int separator_position, lv_event_cb_t event_cb)
{
    lv_obj_t *obj = create_text(parent, icon, txt, LV_MENU_ITEM_BUILDER_VARIANT_1);

    // Create minus button in the next row
    lv_obj_t *btn = lv_btn_create(obj);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);

    // Create the spinbox
    lv_obj_t *spinbox = lv_spinbox_create(obj);
    lv_spinbox_set_range(spinbox, min, max);
    lv_spinbox_set_value(spinbox, val);
    if (digit_count > 0)
    {
        lv_spinbox_set_digit_format(spinbox, digit_count, separator_position);
    }
    

    // Set the style of minus button
    int32_t h = lv_obj_get_height(spinbox);
    lv_obj_set_size(btn, h, h);
    lv_obj_align_to(btn, spinbox, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(btn, lv_spinbox_decrement_event_cb, LV_EVENT_ALL, spinbox);

    // Create and set style of plus button
    btn = lv_btn_create(obj);
    lv_obj_set_size(btn, h, h);
    lv_obj_align_to(btn, spinbox, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(btn, lv_spinbox_increment_event_cb, LV_EVENT_ALL, spinbox);

    // Adujst spinbox width
    lv_obj_set_flex_grow(spinbox, 1);

    // Move Cursor to first non-decimal digit
    lv_spinbox_set_cursor_pos(spinbox, separator_position);

    if (event_cb != NULL)
    {
        lv_obj_add_event_cb(spinbox, event_cb, LV_EVENT_ALL, NULL);
    }

    return obj;
}

static lv_obj_t *create_dropdown(lv_obj_t *parent, const char *icon, const char *txt, const char *options,
                               uint16_t selected, lv_event_cb_t event_cb)
{
    lv_obj_t *obj = create_text(parent, icon, txt, LV_MENU_ITEM_BUILDER_VARIANT_1);

    lv_obj_t *dp = lv_dropdown_create(obj);
    lv_obj_add_flag(dp, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    lv_obj_set_flex_grow(dp, 1);
    lv_dropdown_set_options(dp, options);
    lv_dropdown_set_selected(dp, selected);

    if (event_cb != NULL)
    {
        lv_obj_add_event_cb(dp, event_cb, LV_EVENT_ALL, NULL);
    }

    return obj;
}

static lv_obj_t *create_switch(lv_obj_t *parent, const char *icon, const char *txt, bool chk, lv_event_cb_t event_cb)
{
    lv_obj_t *obj = create_text(parent, icon, txt, LV_MENU_ITEM_BUILDER_VARIANT_1);

    lv_obj_t *sw = lv_switch_create(obj);
    lv_obj_add_state(sw, chk ? LV_STATE_CHECKED : 0);

    if (event_cb)
    {
        lv_obj_add_event_cb(sw, event_cb, LV_EVENT_ALL, NULL);
    }

    return obj;
}

static lv_obj_t *create_btn(lv_obj_t *parent, const char *icon, const char *txt, const char *btn_icon, const char *btn_txt, lv_event_cb_t event_cb)
{
    lv_obj_t *obj = create_text(parent, icon, txt, LV_MENU_ITEM_BUILDER_VARIANT_1);

    lv_obj_t *btn = lv_btn_create(obj);
    // lv_obj_set_size(btn, 44, 44);
    if (btn_icon)
    {
        lv_obj_set_style_bg_img_src(btn, btn_icon, 0);
    }
    else if (btn_txt)
    {
        lv_obj_t * lbl = lv_label_create(btn);
        lv_label_set_text(lbl, btn_txt);
        lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, 0);
    }

    if (event_cb)
    {
        lv_obj_add_event_cb(btn, event_cb, LV_EVENT_ALL, NULL);
    }

    return obj;
}

void ui_SettingScreen_screen_init(void)
{
    ui_SettingScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_SettingScreen, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    lv_obj_t *menu = lv_menu_create(ui_SettingScreen);

    lv_color_t bg_color = lv_obj_get_style_bg_color(menu, 0);
    if (lv_color_brightness(bg_color) > 127)
    {
        lv_obj_set_style_bg_color(menu, lv_color_darken(lv_obj_get_style_bg_color(menu, 0), 10), 0);
    }
    else
    {
        lv_obj_set_style_bg_color(menu, lv_color_darken(lv_obj_get_style_bg_color(menu, 0), 50), 0);
    }
    lv_menu_set_mode_root_back_btn(menu, LV_MENU_ROOT_BACK_BTN_ENABLED);
    lv_obj_add_event_cb(menu, back_event_handler, LV_EVENT_CLICKED, menu);
    lv_obj_set_size(menu, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
    lv_obj_center(menu);

    lv_obj_t *cont;
    lv_obj_t *section;

    /*Create wifi page*/
    ui_setting_wifi_page = lv_menu_page_create(menu, _("Wi-Fi"));
    lv_obj_set_style_pad_hor(ui_setting_wifi_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(ui_setting_wifi_page);
    section = lv_menu_section_create(ui_setting_wifi_page);
    ui_setting_wifi_switch = create_switch(section, LV_SYMBOL_WIFI, _("Wi-Fi"), false, NULL);

    /*Create ble page*/
    ui_setting_ble_page = lv_menu_page_create(menu, _("BLE"));
    lv_obj_set_style_pad_hor(ui_setting_ble_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(ui_setting_ble_page);
    section = lv_menu_section_create(ui_setting_ble_page);

    /*Create usb page*/
    ui_setting_usb_page = lv_menu_page_create(menu, _("USB"));
    lv_obj_set_style_pad_hor(ui_setting_usb_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(ui_setting_usb_page);
    section = lv_menu_section_create(ui_setting_usb_page);
    ui_setting_usb_mcode_tick_rate = create_spinbox(section, NULL, _("Mcode Tickrate"), 1, 256, 96, 3, 0, ui_event_mcodeTickRate);

    /*Create x axis page*/
    ui_setting_x_axis_page = lv_menu_page_create(menu, _("X Axis"));
    lv_obj_set_style_pad_hor(ui_setting_x_axis_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(ui_setting_x_axis_page);

    create_text(ui_setting_x_axis_page, NULL, _("Mechanical"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(ui_setting_x_axis_page);
    ui_setting_x_lead_length = create_spinbox(section, NULL, _("Lead Length (mm)"), 1, 9999, 2000, 4, 2, ui_event_XLeadLength);
    ui_setting_x_operational_speed = create_spinbox(section, NULL, _("Operational Speed (mm/s)"), 1, 9999, 2000, 4, 2, ui_event_XOperationalSpeed);
    ui_setting_x_length_type = create_dropdown(section, NULL, _("Length Type"), _("_length_type_options"), 0, ui_event_XLengthType);
    ui_setting_x_reverse_axis = create_switch(section, NULL, _("Reverse Axis"), false, ui_event_XReverseAxis);

    create_text(ui_setting_x_axis_page, NULL, _("Electrical"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(ui_setting_x_axis_page);
    ui_setting_x_operational_current = create_spinbox(section, NULL, _("Operational Current (mA)"), 1, 2048, 1024, 4, 0, ui_event_XOperationalCurrent);
    ui_setting_x_idle_behavior = create_dropdown(section, NULL, _("Idle Behavior"), _("_idle_behavior_options"), 0, ui_event_XIdleBehavior);
    ui_setting_x_sleep_current = create_spinbox(section, NULL, _("Sleep Current (mA)"), 1, 2048, 512, 4, 0, ui_event_XSleepCurrent);

    create_text(ui_setting_x_axis_page, NULL, _("Zeroing"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(ui_setting_x_axis_page);
    ui_setting_x_auto_zreoing = create_switch(section, NULL, _("Auto Zeroing on Boot"), false, ui_event_XAutoZreoing);
    ui_setting_x_zreoing_reverse_dir = create_switch(section, NULL, _("Reverse Zeroing Direction"), false, ui_event_XZeroingReverseDir);
    ui_setting_x_zeroing_torch_thres = create_spinbox(section, NULL, _("Zeroing Torch Threshold"), 1, 255, 127, 3, 0, ui_event_XZeroingTorchThres);
    ui_setting_x_zeroing_current = create_spinbox(section, NULL, _("Zeroing Current (mA)"), 1, 2048, 512, 4, 0, ui_event_XZeroingCurrent);
    ui_setting_x_zeroing_speed = create_spinbox(section, NULL, _("Zeroing Speed (mm/s)"), 1, 9999, 2000, 4, 2, ui_event_XZeroingSpeed);
    ui_setting_x_zeroing_position = create_spinbox(section, NULL, _("Zeroing Position (mm)"), -9999, 9999, 0, 4, 2, ui_event_XZeroingPosition);
    ui_setting_x_zeroing_start = create_btn(section, NULL, _("Auto Zeroing"), NULL, _("Execute"), ui_event_XZeroingStart);

    /* X axis utility */
    create_text(ui_setting_x_axis_page, NULL, _("Utility"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(ui_setting_x_axis_page);
    {
        lv_obj_t * menu = lv_menu_cont_create(section);

        ui_setting_x_util_move_left = lv_btn_create(menu);
        lv_obj_set_size(ui_setting_x_util_move_left, 44, 44);
        lv_obj_set_style_bg_img_src(ui_setting_x_util_move_left, LV_SYMBOL_LEFT, 0);

        lv_obj_t * label = lv_label_create(menu);
        lv_label_set_text(label, _("Move Axis"));

        ui_setting_x_util_move_right = lv_btn_create(menu);
        lv_obj_set_size(ui_setting_x_util_move_right, 44, 44);
        lv_obj_set_style_bg_img_src(ui_setting_x_util_move_right, LV_SYMBOL_RIGHT, 0);

        lv_obj_set_flex_grow(label, 1);
        lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);

        lv_obj_add_event_cb(ui_setting_x_util_move_left, ui_event_XUtilMove, LV_EVENT_ALL, 1);
        lv_obj_add_event_cb(ui_setting_x_util_move_right, ui_event_XUtilMove, LV_EVENT_ALL, NULL);
    }
    ui_setting_x_util_sg_result = create_text(section, NULL, _("SG Result"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    ui_setting_x_util_speed_type = create_dropdown(section, NULL, _("Speed Type"), _("_move_axis_speed_options"), 0, NULL);


    /*Create y axis page*/
    ui_setting_y_axis_page = lv_menu_page_create(menu, _("Y Axis"));
    lv_obj_set_style_pad_hor(ui_setting_y_axis_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(ui_setting_y_axis_page);

    create_text(ui_setting_y_axis_page, NULL, _("Mechanical"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(ui_setting_y_axis_page);
    ui_setting_y_lead_length = create_spinbox(section, NULL, _("Lead Length (mm)"), 1, 9999, 2000, 4, 2, ui_event_YLeadLength);
    ui_setting_y_operational_speed = create_spinbox(section, NULL, _("Operational Speed (mm/s)"), 1, 9999, 2000, 4, 2, ui_event_YOperationalSpeed);
    ui_setting_y_length_type = create_dropdown(section, NULL, _("Length Type"), _("_length_type_options"), 0, ui_event_YLengthType);
    ui_setting_y_reverse_axis = create_switch(section, NULL, _("Reverse Axis"), false, ui_event_YReverseAxis);
    ui_setting_y_punch_depth = create_spinbox(section, NULL, _("Punch Depth (mm)"), -999, 999, 0, 3, 1, ui_event_YPunchDepth);

    create_text(ui_setting_y_axis_page, NULL, _("Electrical"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(ui_setting_y_axis_page);
    ui_setting_y_operational_current = create_spinbox(section, NULL, _("Operational Current (mA)"), 1, 2048, 1024, 4, 0, ui_event_YOperationalCurrent);
    ui_setting_y_idle_behavior = create_dropdown(section, NULL, _("Idle Behavior"), _("_idle_behavior_options"), 0, ui_event_YIdleBehavior);
    ui_setting_y_sleep_current = create_spinbox(section, NULL, _("Sleep Current (mA)"), 1, 2048, 512, 4, 0, ui_event_YSleepCurrent);

    create_text(ui_setting_y_axis_page, NULL, _("Zeroing"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(ui_setting_y_axis_page);
    ui_setting_y_auto_zreoing = create_switch(section, NULL, _("Auto Zeroing on Boot"), false, ui_event_YAutoZreoing);
    ui_setting_y_zreoing_reverse_dir = create_switch(section, NULL, _("Reverse Zeroing Direction"), false, ui_event_YZeroingReverseDir);
    ui_setting_y_zeroing_torch_thres = create_spinbox(section, NULL, _("Zeroing Torch Threshold"), 1, 255, 127, 3, 0, ui_event_YZeroingTorchThres);
    ui_setting_y_zeroing_current = create_spinbox(section, NULL, _("Zeroing Current (mA)"), 1, 2048, 512, 4, 0, ui_event_YZeroingCurrent);
    ui_setting_y_zeroing_speed = create_spinbox(section, NULL, _("Zeroing Speed (mm/s)"), 1, 9999, 2000, 4, 2, ui_event_YZeroingSpeed);
    ui_setting_y_zeroing_position = create_spinbox(section, NULL, _("Zeroing Position (mm)"), -9999, 9999, 0, 4, 2, ui_event_YZeroingPosition);
    ui_setting_y_zeroing_start = create_btn(section, NULL, _("Auto Zeroing"), NULL, _("Execute"), ui_event_YZeroingStart);

    /* Y axis utility */
    create_text(ui_setting_y_axis_page, NULL, _("Utility"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(ui_setting_y_axis_page);
    {
        lv_obj_t * menu = lv_menu_cont_create(section);

        ui_setting_y_util_move_up = lv_btn_create(menu);
        lv_obj_set_size(ui_setting_y_util_move_up, 44, 44);
        lv_obj_set_style_bg_img_src(ui_setting_y_util_move_up, LV_SYMBOL_UP, 0);

        lv_obj_t * label = lv_label_create(menu);
        lv_label_set_text(label, _("Move Axis"));

        ui_setting_y_util_move_down = lv_btn_create(menu);
        lv_obj_set_size(ui_setting_y_util_move_down, 44, 44);
        lv_obj_set_style_bg_img_src(ui_setting_y_util_move_down, LV_SYMBOL_DOWN, 0);

        lv_obj_set_flex_grow(label, 1);
        lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);

        lv_obj_add_event_cb(ui_setting_y_util_move_up, ui_event_YUtilMove, LV_EVENT_ALL, 1);
        lv_obj_add_event_cb(ui_setting_y_util_move_down, ui_event_YUtilMove, LV_EVENT_ALL, NULL);
    }
    ui_setting_y_util_sg_result = create_text(section, NULL, _("SG Result"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    ui_setting_y_util_speed_type = create_dropdown(section, NULL, _("Speed Type"), _("_move_axis_speed_options"), 0, NULL);
    
    /*Create z aixs page*/
    ui_setting_z_axis_page = lv_menu_page_create(menu, _("Z Axis"));
    lv_obj_set_style_pad_hor(ui_setting_z_axis_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(ui_setting_z_axis_page);
    
    create_text(ui_setting_z_axis_page, NULL, _("Mechanical"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(ui_setting_z_axis_page);
    ui_setting_z_lead_length = create_spinbox(section, NULL, _("Lead Length (mm)"), 1, 9999, 2000, 4, 2, ui_event_ZLeadLength);
    ui_setting_z_operational_speed = create_spinbox(section, NULL, _("Operational Speed (mm/s)"), 1, 9999, 2000, 4, 2, ui_event_ZOperationalSpeed);
    ui_setting_z_length_type = create_dropdown(section, NULL, _("Length Type"), _("_length_type_options"), 0, ui_event_ZLengthType);
    ui_setting_z_reverse_axis = create_switch(section, NULL, _("Reverse Axis"), false, ui_event_ZReverseAxis);

    create_text(ui_setting_z_axis_page, NULL, _("Electrical"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(ui_setting_z_axis_page);
    ui_setting_z_operational_current = create_spinbox(section, NULL, _("Operational Current (mA)"), 1, 2048, 1024, 4, 0, ui_event_ZOperationalCurrent);
    ui_setting_z_idle_behavior = create_dropdown(section, NULL, _("Idle Behavior"), _("_idle_behavior_options"), 0, ui_event_ZIdleBehavior);
    ui_setting_z_sleep_current = create_spinbox(section, NULL, _("Sleep Current (mA)"), 1, 2048, 512, 4, 0, ui_event_ZSleepCurrent);

    create_text(ui_setting_z_axis_page, NULL, _("Calibration"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(ui_setting_z_axis_page);
    ui_setting_z_cali_target = create_spinbox(section, NULL, _("Target Beats"), 1, 500, 50, 3, 0, ui_event_ZCaliTarget);
    ui_setting_z_cali_measure = create_spinbox(section, NULL, _("Measured Beats"), 1, 500, 50, 3, 0, ui_event_ZCaliMeasure);
    ui_setting_z_cali_residual = create_spinbox(section, NULL, _("Measured Residual (mm)"), -8000, 8000, 0, 4, 1, ui_event_ZCaliResidual);

    create_text(ui_setting_z_axis_page, NULL, _("Encoder"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(ui_setting_z_axis_page);
    ui_setting_z_encoder_switch = create_switch(section, NULL, _("Use Encoder"), false, ui_event_ZEncoderSwitch);
    ui_setting_z_encoder_type = create_dropdown(section, NULL, _("Encoder Type"), _("_encoder_type_options"), 0, ui_event_ZEncoderType);


    /*Create power page*/
    ui_setting_power_page = lv_menu_page_create(menu, _("Power"));
    lv_obj_set_style_pad_hor(ui_setting_power_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(ui_setting_power_page);
    // create_text(sub_power_page, NULL, _("Volatge"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(ui_setting_power_page);
    ui_setting_power_voltage = create_dropdown(section, NULL, _("Motor Voltage"), _("_motor_voltage_options"), 0, ui_event_VoltageDropDown);


    /*Create display page*/
    ui_setting_display_page = lv_menu_page_create(menu, _("Display"));
    lv_obj_set_style_pad_hor(ui_setting_display_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(ui_setting_display_page);
    create_text(ui_setting_display_page, NULL, _("Screen"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(ui_setting_display_page);
    ui_setting_display_brightness = create_slider(section, LV_SYMBOL_SETTINGS, _("Brightness"), 1, 255, 128, ui_event_BrightnessSlider);
    create_text(ui_setting_display_page, NULL, _("System"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(ui_setting_display_page);
    ui_setting_display_language = create_dropdown(section, NULL, _("Language"), _("_language_options"), 0, ui_event_LanguageDropDown);

    /*Create about page*/
    ui_setting_about_page = lv_menu_page_create(menu, _("About"));
    lv_obj_set_style_pad_hor(ui_setting_about_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(ui_setting_about_page);
    section = lv_menu_section_create(ui_setting_about_page);
    cont = create_text(section, NULL, _("Project Puncher\nNext Gen"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    cont = create_text(section, NULL, _("Version"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_label_set_text_fmt(lv_obj_get_child(cont, -1), _("_version"), PROJECT_VER);
    cont = create_text(section, NULL, _("_about_page_author"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_label_set_long_mode(lv_obj_get_child(cont, -1), LV_LABEL_LONG_WRAP);

    /*Create a root page*/
    ui_setting_root_page = lv_menu_page_create(menu, NULL);
    lv_obj_set_style_pad_hor(ui_setting_root_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);

    create_text(ui_setting_root_page, NULL, _("Connectivity"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(ui_setting_root_page);
    cont = create_text(section, LV_SYMBOL_WIFI, _("Wi-Fi"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont, ui_setting_wifi_page);
    cont = create_text(section, LV_SYMBOL_BLUETOOTH, _("BLE"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont, ui_setting_ble_page);
    cont = create_text(section, LV_SYMBOL_USB, _("USB"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont, ui_setting_usb_page);

    create_text(ui_setting_root_page, NULL, _("Motors"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(ui_setting_root_page);
    cont = create_text(section, LV_SYMBOL_SETTINGS, _("X Axis"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont, ui_setting_x_axis_page);
    cont = create_text(section, LV_SYMBOL_SETTINGS, _("Y Axis"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont, ui_setting_y_axis_page);
    cont = create_text(section, LV_SYMBOL_SETTINGS, _("Z Axis"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont, ui_setting_z_axis_page);
    cont = create_text(section, LV_SYMBOL_CHARGE, _("Power"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont, ui_setting_power_page);

    create_text(ui_setting_root_page, NULL, _("Display"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(ui_setting_root_page);
    cont = create_text(section, LV_SYMBOL_IMAGE, _("Disp."), LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont, ui_setting_display_page);

    create_text(ui_setting_root_page, NULL, _("Others"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(ui_setting_root_page);
    cont = create_text(section, LV_SYMBOL_FILE, _("About"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont, ui_setting_about_page);

    lv_menu_set_sidebar_page(menu, ui_setting_root_page);

    // increase header height
    lv_obj_t *sidebar_header = lv_menu_get_sidebar_header(menu);
    lv_obj_set_height(sidebar_header, lv_pct(14));
    // change sidebar width
    lv_obj_set_width(lv_obj_get_parent(ui_setting_root_page), lv_pct(34));

    // increase back button size
    lv_obj_t *back_btn = lv_menu_get_sidebar_header_back_btn(menu);
    lv_obj_t *back_btn_label = lv_label_create(back_btn);
    lv_label_set_text(back_btn_label, _("Back"));
    lv_obj_set_flex_grow(back_btn, 1);
    
    lv_event_send(lv_obj_get_child(lv_obj_get_child(lv_menu_get_cur_sidebar_page(menu), 1), 0), LV_EVENT_CLICKED, NULL);
}
