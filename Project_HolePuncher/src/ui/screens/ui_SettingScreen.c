#include "../ui.h"
#include "../ui_events.h"

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
    lv_obj_t *sub_wifi_page = lv_menu_page_create(menu, _("Wi-Fi"));
    lv_obj_set_style_pad_hor(sub_wifi_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_wifi_page);
    section = lv_menu_section_create(sub_wifi_page);
    create_switch(section, LV_SYMBOL_WIFI, _("Wi-Fi"), false, NULL);

    /*Create x axis page*/
    lv_obj_t *sub_x_axis_page = lv_menu_page_create(menu, _("X Axis"));
    lv_obj_set_style_pad_hor(sub_x_axis_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_x_axis_page);

    create_text(sub_x_axis_page, NULL, _("Mechanical"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(sub_x_axis_page);
    create_spinbox(section, NULL, _("Lead Length (mm)"), 1, 4000, 2000, 4, 2, NULL);
    create_switch(section, NULL, _("Reverse Axis"), false, NULL);

    create_text(sub_x_axis_page, NULL, _("Electrical"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(sub_x_axis_page);
    create_spinbox(section, NULL, _("Operational Current (mA)"), 1, 2048, 1024, 4, 0, NULL);
    create_dropdown(section, NULL, _("Idle Behavior"), _("_idle_behavior_options"), 0, NULL);
    create_spinbox(section, NULL, _("Sleep Current(mA)"), 1, 2048, 512, 4, 0, NULL);

    create_text(sub_x_axis_page, NULL, _("Zeroing"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(sub_x_axis_page);
    create_switch(section, NULL, _("Auto Zeroing"), false, NULL);
    create_spinbox(section, NULL, _("Zeroing Torch Threshold"), 1, 255, 127, 3, 0, NULL);
    create_spinbox(section, NULL, _("Zeroing Current (mA)"), 1, 2048, 512, 4, 0, NULL);


    /*Create y axis page*/
    lv_obj_t *sub_y_axis_page = lv_menu_page_create(menu, _("Y Axis"));
    lv_obj_set_style_pad_hor(sub_y_axis_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_y_axis_page);

    create_text(sub_y_axis_page, NULL, _("Mechanical"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(sub_y_axis_page);
    create_spinbox(section, NULL, _("Lead Length (mm)"), 1, 4000, 2000, 4, 2, NULL);
    create_switch(section, NULL, _("Reverse Axis"), false, NULL);

    create_text(sub_y_axis_page, NULL, _("Electrical"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(sub_y_axis_page);
    create_spinbox(section, NULL, _("Operational Current (mA)"), 1, 2048, 1024, 4, 0, NULL);
    create_dropdown(section, NULL, _("Idle Behavior"), _("_idle_behavior_options"), 0, NULL);
    create_spinbox(section, NULL, _("Sleep Current(mA)"), 1, 2048, 512, 4, 0, NULL);

    create_text(sub_y_axis_page, NULL, _("Zeroing"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(sub_y_axis_page);
    create_switch(section, NULL, _("Auto Zeroing"), false, NULL);
    create_spinbox(section, NULL, _("Zeroing Torch Threshold"), 1, 255, 127, 3, 0, NULL);
    create_spinbox(section, NULL, _("Zeroing Current (mA)"), 1, 2048, 512, 4, 0, NULL);
    
    /*Create z aixs page*/
    lv_obj_t *sub_z_axis_page = lv_menu_page_create(menu, _("Z Axis"));
    lv_obj_set_style_pad_hor(sub_z_axis_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_z_axis_page);
    
    create_text(sub_z_axis_page, NULL, _("Mechanical"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(sub_z_axis_page);
    create_spinbox(section, NULL, _("Lead Length (mm)"), 1, 4000, 2000, 4, 2, NULL);
    create_switch(section, NULL, _("Reverse Axis"), false, NULL);

    create_text(sub_z_axis_page, NULL, _("Electrical"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(sub_z_axis_page);
    create_spinbox(section, NULL, _("Operational Current (mA)"), 1, 2048, 1024, 4, 0, NULL);
    create_dropdown(section, NULL, _("Idle Behavior"), _("_idle_behavior_options"), 0, NULL);
    create_spinbox(section, NULL, _("Sleep Current (mA)"), 1, 2048, 512, 4, 0, NULL);

    /*Create display page*/
    lv_obj_t *sub_display_page = lv_menu_page_create(menu, _("Display"));
    lv_obj_set_style_pad_hor(sub_display_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_display_page);
    section = lv_menu_section_create(sub_display_page);
    create_slider(section, LV_SYMBOL_SETTINGS, _("Brightness"), 1, 255, 128, ui_event_BrightnessSlider);

    /*Create about page*/
    lv_obj_t *sub_about_page = lv_menu_page_create(menu, _("About"));
    lv_obj_set_style_pad_hor(sub_about_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);
    lv_menu_separator_create(sub_about_page);
    section = lv_menu_section_create(sub_about_page);
    cont = create_text(section, NULL, "Project Puncher\nNext Gen", LV_MENU_ITEM_BUILDER_VARIANT_1);
    cont = create_text(section, NULL, "Version 0.0", LV_MENU_ITEM_BUILDER_VARIANT_1);

    /*Create a root page*/
    root_page = lv_menu_page_create(menu, _("Setting"));
    lv_obj_set_style_pad_hor(root_page, lv_obj_get_style_pad_left(lv_menu_get_main_header(menu), 0), 0);

    create_text(root_page, NULL, _("Connectivity"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(root_page);
    cont = create_text(section, LV_SYMBOL_WIFI, _("Wi-Fi"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont, sub_wifi_page);
    cont = create_text(section, LV_SYMBOL_BLUETOOTH, _("BLE"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    cont = create_text(section, LV_SYMBOL_USB, _("USB"), LV_MENU_ITEM_BUILDER_VARIANT_1);

    create_text(root_page, NULL, _("Motors"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(root_page);
    cont = create_text(section, LV_SYMBOL_SETTINGS, _("X Axis"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont, sub_x_axis_page);
    cont = create_text(section, LV_SYMBOL_SETTINGS, _("Y Axis"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont, sub_y_axis_page);
    cont = create_text(section, LV_SYMBOL_SETTINGS, _("Z Axis"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont, sub_z_axis_page);

    create_text(root_page, NULL, _("Display"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(root_page);
    cont = create_text(section, LV_SYMBOL_IMAGE, _("Disp."), LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont, sub_display_page);

    create_text(root_page, NULL, _("Others"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    section = lv_menu_section_create(root_page);
    cont = create_text(section, LV_SYMBOL_FILE, _("About"), LV_MENU_ITEM_BUILDER_VARIANT_1);
    lv_menu_set_load_page_event(menu, cont, sub_about_page);

    lv_menu_set_sidebar_page(menu, root_page);

    // increase header height
    lv_obj_t *sidebar_header = lv_menu_get_sidebar_header(menu);
    lv_obj_set_height(sidebar_header, lv_pct(14));
    // change sidebar width
    lv_obj_set_width(lv_obj_get_parent(root_page), lv_pct(34));

    lv_event_send(lv_obj_get_child(lv_obj_get_child(lv_menu_get_cur_sidebar_page(menu), 1), 0), LV_EVENT_CLICKED, NULL);
}