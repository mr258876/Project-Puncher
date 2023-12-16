#ifndef _LVGL_PUNCHERUI_H_
#define _LVGL_PUNCHERUI_H_

#include "PuncherUI.h"
#include <lvgl.h>

class LVGLPuncherUI : public PuncherUI
{
public:
    lv_disp_drv_t disp_drv;
    lv_disp_draw_buf_t draw_buf;
    lv_color_t *disp_draw_buf;

    LVGLPuncherUI();
    ~LVGLPuncherUI();

    void begin();
    void onEvent(puncher_event_t *msg);
    
    void onSettingValueChange(puncher_event_setting_change_t *msg);
    void setBrightness(int brightness);
};

extern LVGLPuncherUI *lvgl_ui;

#endif  // _LVGL_PUNCHERUI_H_