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
    void alert(ui_alert_msg_t *msg);
    void drawMenu(std::vector<ui_menu_item_t>, void* ui_param=NULL);
};

#endif  // _LVGL_PUNCHERUI_H_