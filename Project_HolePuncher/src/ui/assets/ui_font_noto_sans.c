/**
 * @file ui_font_noto_sans.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include "PuncherConf.h"
#include "ui_font_noto_sans.h"

/*********************
 *      DEFINES
 *********************/
/* FreeType uses C standard file system, so no driver letter is required.
 * But on ESP, a mount point needs to be specified
 */
#define MOUNT_POINT FLASH_MOUNT_POINT

/* Store font files in the file system of the SD card
 *[Important] No need to specify a drive letter, but a mount point needs to be specified
 */
#define FREETYPE_FONT_FILE (MOUNT_POINT "/NotoSans-Pruned.ttf")

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

lv_font_t *ui_font_noto_sans_16;
lv_font_t *ui_font_noto_sans_36;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Load a font with FreeType
 */
void lv_font_notosans_init(void)
{
    /*FreeType uses C standard file system, so no driver letter is required.*/
    lv_ft_info_t ft_font_noto_sans_16 = {
        .name = FREETYPE_FONT_FILE,
        .weight = 16,
        .style = FT_FONT_STYLE_NORMAL,
        .mem = NULL,
    };
    if (!lv_ft_font_init(&ft_font_noto_sans_16))
    {
        LV_LOG_ERROR("create failed.");
    }
    ui_font_noto_sans_16 = ft_font_noto_sans_16.font;
    ui_font_noto_sans_16->fallback = &lv_font_montserrat_16;

    lv_ft_info_t ft_font_noto_sans_36 = {
        .name = FREETYPE_FONT_FILE,
        .weight = 36,
        .style = FT_FONT_STYLE_NORMAL,
        .mem = NULL,
    };
    if (!lv_ft_font_init(&ft_font_noto_sans_36))
    {
        LV_LOG_ERROR("create failed.");
    }
    ui_font_noto_sans_36 = ft_font_noto_sans_36.font;
    ui_font_noto_sans_36->fallback = &lv_font_montserrat_36;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
