#include "LVGL_PuncherUI.h"
#include "PuncherConf.h"

#include "ui/ui.h"
#include "ui/lv_port_encoder.h"

LVGLPuncherUI::LVGLPuncherUI()
{   
    lv_init();

    /* Initialize the display */
    lv_disp_drv_init(&disp_drv);
}

LVGLPuncherUI::~LVGLPuncherUI()
{
}

void LVGLPuncherUI::begin()
{
    disp_draw_buf = (lv_color_t *)malloc(LV_DISP_BUF_SIZE);

    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, SCREEN_WIDTH * SCREEN_HEIGHT);

    disp_drv.hor_res = SCREEN_WIDTH;
    disp_drv.ver_res = SCREEN_HEIGHT;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    /* Initialize input device driver */
    lv_encoder_indev_init();
    
    ui_init();

    LV_LOG_INFO("LVGL Booted.");
}

void LVGLPuncherUI::alert(ui_alert_msg_t *msg)
{
    
}

void LVGLPuncherUI::drawMenu(std::vector<ui_menu_item_t>, void* ui_param)
{

}
