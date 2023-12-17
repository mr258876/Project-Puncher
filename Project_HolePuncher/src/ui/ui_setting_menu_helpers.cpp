#include "ui_setting_menu_helpers.h"
#include "PuncherSemaphore.h"
#include "i18n/lv_i18n.h"

void _ui_menu_switch_set_value(lv_obj_t *obj, std::any param)
{
    lv_obj_t *sw = lv_obj_get_child(obj, -1);

    uint8_t val = std::any_cast<uint8_t>(param);
    xSemaphoreTake(LVGLMutex, portMAX_DELAY);
    if (val)
    {
        lv_obj_add_state(sw, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_clear_state(sw, LV_STATE_CHECKED);
    }
    xSemaphoreGive(LVGLMutex);
}

void _ui_menu_roller_set_value(lv_obj_t *obj, std::any param)
{
    uint16_t val = std::any_cast<uint16_t>(param);
    xSemaphoreTake(LVGLMutex, portMAX_DELAY);
    // lv_roller_set_selected(obj, val, LV_ANIM_ON);
    xSemaphoreGive(LVGLMutex);
}

void _ui_menu_dropdown_set_value(lv_obj_t *obj, std::any param)
{
    lv_obj_t *dp = lv_obj_get_child(obj, -1);

    uint16_t val = std::any_cast<uint16_t>(param);
    xSemaphoreTake(LVGLMutex, portMAX_DELAY);
    lv_dropdown_set_selected(dp, val);
    xSemaphoreGive(LVGLMutex);
}

void _ui_menu_spinbox_set_value(lv_obj_t *obj, std::any param)
{
    lv_obj_t *spinbox = lv_obj_get_child(obj, -2);

    int32_t val = std::any_cast<int32_t>(param);
    xSemaphoreTake(LVGLMutex, portMAX_DELAY);
    lv_spinbox_set_value(spinbox, val);
    lv_obj_set_flex_grow(spinbox, 1);   // Adujst spinbox width
    xSemaphoreGive(LVGLMutex);
}

void _ui_menu_set_language(lv_obj_t *obj, std::any param)
{
    uint16_t val = std::any_cast<uint16_t>(param);
    const char *locale = lv_i18n_language_pack[val]->locale_name;
    xSemaphoreTake(LVGLMutex, portMAX_DELAY);
    lv_i18n_set_locale(locale);
    lv_obj_invalidate(lv_scr_act());
    xSemaphoreGive(LVGLMutex);
}
