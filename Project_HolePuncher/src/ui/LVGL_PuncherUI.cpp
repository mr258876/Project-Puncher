#include "LVGL_PuncherUI.h"
#include "PuncherConf.h"

#include "ui/ui.h"
#include "ui/lv_port.h"
#include "ui/lv_port_indev_ctp.h"

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
    lv_port_init();

    /* Initialize input device driver */
    lv_port_indev_init();
    
    ui_init();

    LV_LOG_INFO("LVGL Booted.");
}

void LVGLPuncherUI::alert(ui_alert_msg_t *msg)
{
    
}

void LVGLPuncherUI::drawMenu(std::vector<ui_menu_item_t>, void* ui_param)
{

}

LVGLPuncherUI *lvgl_ui = new LVGLPuncherUI();