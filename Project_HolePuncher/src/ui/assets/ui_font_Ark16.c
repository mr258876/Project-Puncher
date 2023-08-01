/*******************************************************************************
 * Size: 16 px
 * Bpp: 2
 * Opts: 
 ******************************************************************************/


#include "lvgl.h"


#ifndef UI_FONT_ARK16
#define UI_FONT_ARK16 1
#endif

#if UI_FONT_ARK16

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */

    /* U+0021 "!" */
    0x33, 0x30, 0xf6, 0x61, 0xfe, 0xc3, 0xb0,

    /* U+0022 "\"" */
    0xcc, 0x3c,

    /* U+0023 "#" */
    0x30, 0x61, 0xf6, 0x78, 0xf1, 0xe3, 0xf, 0xfe,
    0x66, 0x78, 0xf1, 0xe3, 0xf, 0x0,

    /* U+0024 "$" */
    0xb, 0xd, 0xe3, 0xc7, 0xc7, 0xc1, 0xec, 0x3e,
    0xf0, 0x7b, 0xc6, 0x1b, 0x3c, 0x1e, 0xf0, 0x7d,
    0x87, 0xbe, 0x3e, 0x3c, 0x78,

    /* U+0025 "%" */
    0x30, 0xb3, 0xe0, 0xdf, 0x1e, 0x6, 0x1f, 0xbc,
    0x1f, 0xfc, 0x6f, 0x7, 0xec, 0x1e, 0x3e, 0xd,
    0xf0,

    /* U+0026 "&" */
    0x7, 0x83, 0x7e, 0xf, 0xfe, 0x2f, 0xe0, 0xbc,
    0x1f, 0x78, 0xf8, 0x3f, 0xbe, 0xf, 0xbf, 0xe0,

    /* U+0027 "'" */
    0xc0,

    /* U+0028 "(" */
    0x6, 0x78, 0x2f, 0x7, 0xff, 0x2b, 0xc1, 0xde,

    /* U+0029 ")" */
    0xc1, 0xe0, 0xef, 0x7, 0xff, 0x2b, 0xc1, 0x78,

    /* U+002A "*" */
    0x6, 0xc, 0x2f, 0x8f, 0xc7, 0xc1, 0x60,

    /* U+002B "+" */
    0x6, 0x1f, 0x78, 0xfc, 0x78, 0x38,

    /* U+002C "," */
    0xf0, 0x33, 0xc0,

    /* U+002D "-" */
    0xfe,

    /* U+002E "." */
    0xf0, 0x0,

    /* U+002F "/" */
    0xd, 0x87, 0xff, 0x7, 0xc1, 0xff, 0xc0, 0xf0,
    0x7d, 0xe0, 0xff, 0xe0, 0x78, 0x3f, 0xf8, 0x20,

    /* U+0030 "0" */
    0x3f, 0x1f, 0xe0, 0xff, 0xe2, 0x61, 0x7c, 0xf,
    0x82, 0xc3, 0xff, 0x89, 0xfe,

    /* U+0031 "1" */
    0xb, 0xd, 0x85, 0xf0, 0x5e, 0xf, 0xfe, 0xe7,
    0xc7, 0x80,

    /* U+0032 "2" */
    0x3f, 0x1f, 0xf0, 0x7f, 0xf2, 0x7c, 0x17, 0x82,
    0xf0, 0x5e, 0xb, 0xc1, 0xdf, 0xc0,

    /* U+0033 "3" */
    0x3f, 0x1f, 0xf0, 0x7f, 0xf1, 0xff, 0x3, 0xf0,
    0x7f, 0xf0, 0x30, 0xef, 0xf0,

    /* U+0034 "4" */
    0xb, 0xc1, 0x78, 0x3f, 0xbc, 0x1f, 0xfc, 0x3f,
    0x7, 0xfd, 0xf1, 0xfc, 0x61, 0xe0,

    /* U+0035 "5" */
    0xff, 0x1f, 0xc1, 0xff, 0xc7, 0xfc, 0x7f, 0x83,
    0xff, 0x81, 0x87, 0x7f, 0x80,

    /* U+0036 "6" */
    0x3f, 0x1f, 0xe0, 0xec, 0x3f, 0xf8, 0x3f, 0x81,
    0xfc, 0x1f, 0xfc, 0x6f, 0xf0,

    /* U+0037 "7" */
    0xff, 0xf8, 0x3f, 0xef, 0x7, 0xff, 0xf, 0xc1,
    0xff, 0xcd,

    /* U+0038 "8" */
    0x3f, 0x1f, 0xe0, 0xff, 0xe3, 0x7f, 0xff, 0x0,
    0x3f, 0xf8, 0xdf, 0xe0,

    /* U+0039 "9" */
    0x3f, 0x1f, 0xe0, 0xff, 0xe3, 0x7f, 0x3, 0xf0,
    0x7f, 0xf0, 0x70, 0xef, 0xf0,

    /* U+003A ":" */
    0xf0, 0x3c, 0x1e, 0xf0, 0x0,

    /* U+003B ";" */
    0xf0, 0x3c, 0x1e, 0xf0, 0x33, 0xc0,

    /* U+003C "<" */
    0xb, 0x7, 0x8f, 0x1e, 0x7, 0x82, 0xf0, 0x5e,

    /* U+003D "=" */
    0xff, 0xfe, 0x18,

    /* U+003E ">" */
    0xc2, 0xf0, 0x5e, 0xb, 0xc0, 0xf1, 0xe3, 0xc0,

    /* U+003F "?" */
    0x3f, 0x1f, 0xe0, 0xf6, 0x1f, 0xde, 0xb, 0xc1,
    0xff, 0xc4, 0xc3, 0xfe, 0xc0,

    /* U+0040 "@" */
    0x7, 0xe0, 0xbf, 0xc7, 0x83, 0x78, 0x2f, 0x83,
    0x7c, 0x1f, 0xfd, 0x2f, 0xc1, 0x7c, 0x7e, 0xf,
    0xbf, 0xc0,

    /* U+0041 "A" */
    0xb, 0xf, 0xfe, 0x7, 0xc1, 0xff, 0xc5, 0xf1,
    0xe0, 0xff, 0x7c, 0xf, 0xf8, 0x3f, 0xf8, 0x40,

    /* U+0042 "B" */
    0xfe, 0xb, 0xf8, 0x3d, 0xe0, 0xff, 0xe0, 0x78,
    0xfe, 0x7, 0xf0, 0x7b, 0xc1, 0xff, 0xc0, 0xf1,
    0xfc, 0x0,

    /* U+0043 "C" */
    0x7, 0xe0, 0x7f, 0xe0, 0xd8, 0x7f, 0xf7, 0xbc,
    0x1b, 0x3f, 0xc0,

    /* U+0044 "D" */
    0xfe, 0xb, 0xf8, 0x3d, 0xe0, 0xff, 0xf1, 0xf8,
    0xfe, 0x0,

    /* U+0045 "E" */
    0xff, 0x8f, 0xf0, 0x7f, 0xf2, 0xbf, 0x81, 0xfc,
    0x1f, 0xfc, 0xbf, 0xf0,

    /* U+0046 "F" */
    0xff, 0x8f, 0xf0, 0x7f, 0xf2, 0xbf, 0x81, 0xfc,
    0x1f, 0xfd, 0x10,

    /* U+0047 "G" */
    0x7, 0xe0, 0x7f, 0xe0, 0xd8, 0x7f, 0xf3, 0x7e,
    0xd, 0xe0, 0xfd, 0xe0, 0xf7, 0xf0,

    /* U+0048 "H" */
    0xc3, 0xb0, 0xff, 0xe8, 0xff, 0x3, 0xf8, 0x3f,
    0xfa, 0x20,

    /* U+0049 "I" */
    0xfc, 0x61, 0xff, 0xcc, 0xcc,

    /* U+004A "J" */
    0xe, 0xc3, 0xff, 0xb1, 0x87, 0xff, 0x17, 0xfc,

    /* U+004B "K" */
    0xc3, 0xb0, 0xef, 0x6, 0xf0, 0x6f, 0x6, 0xf0,
    0x6f, 0x7, 0x78, 0x3d, 0xe0, 0xf7, 0x83, 0xde,
    0xf, 0x78,

    /* U+004C "L" */
    0xc3, 0xff, 0xe1, 0xfc,

    /* U+004D "M" */
    0xc3, 0xfb, 0x30, 0xfb, 0xf, 0xfe, 0xf, 0x83,
    0xbc, 0x1f, 0xfc, 0x2f, 0x5, 0xe0, 0xff, 0xe2,
    0x78, 0xf0, 0x7f, 0xf1, 0xbe, 0xf, 0xfe, 0x18,

    /* U+004E "N" */
    0xc3, 0xb3, 0xf, 0xfe, 0xf, 0x83, 0xff, 0x83,
    0xe0, 0xf7, 0x83, 0xff, 0x83, 0xe0, 0xff, 0xe0,
    0xe0,

    /* U+004F "O" */
    0x7, 0xc1, 0x7f, 0x1e, 0xb, 0xc1, 0xff, 0xde,
    0xf0, 0x5e, 0x3f, 0x80,

    /* U+0050 "P" */
    0xff, 0x3, 0xfc, 0x1f, 0xfc, 0xaf, 0xf1, 0xfc,
    0x1f, 0xfd, 0x10,

    /* U+0051 "Q" */
    0x7, 0xc1, 0x7f, 0x1e, 0xb, 0xc1, 0xff, 0xdb,
    0xc1, 0xe0, 0x7c, 0x7f, 0x80,

    /* U+0052 "R" */
    0xff, 0x3, 0xfc, 0x1f, 0xfc, 0xaf, 0xf1, 0xe3,
    0xc1, 0xbc, 0x1f, 0xfc, 0x1f, 0x7, 0xbc,

    /* U+0053 "S" */
    0x3f, 0x8f, 0xf8, 0x3d, 0x87, 0xde, 0xf, 0x7e,
    0xd, 0xf8, 0x3d, 0xe0, 0xfb, 0xf, 0x7f, 0xc0,

    /* U+0054 "T" */
    0xff, 0xf1, 0xf0, 0x7f, 0xfc, 0xc0,

    /* U+0055 "U" */
    0xc3, 0xb0, 0xff, 0xf6, 0x78, 0x2f, 0x1f, 0xc0,

    /* U+0056 "V" */
    0xc3, 0xb0, 0xff, 0xe1, 0x78, 0x2f, 0x7, 0xff,
    0xf, 0xc7, 0x83, 0xff, 0x8b, 0xf0, 0x7f, 0x0,

    /* U+0057 "W" */
    0xc3, 0x61, 0xb0, 0xff, 0xe1, 0xfc, 0x1f, 0xfc,
    0x3f, 0x7, 0xde, 0xb, 0xc7, 0x83, 0xff, 0x9d,
    0xf0, 0x5f, 0x7, 0xff, 0x2c,

    /* U+0058 "X" */
    0xc3, 0xb0, 0xfb, 0xc1, 0x78, 0x3f, 0x78, 0xf0,
    0x5f, 0x6, 0xf8, 0x2f, 0x1e, 0xf, 0xde, 0xb,
    0xc1, 0xf0,

    /* U+0059 "Y" */
    0xc3, 0xb0, 0xfb, 0xc1, 0x78, 0x3f, 0x78, 0xf0,
    0x7f, 0xbe, 0xf, 0xfe, 0x90,

    /* U+005A "Z" */
    0xff, 0xfe, 0x8, 0x7b, 0xc1, 0xbc, 0x1f, 0xf7,
    0x83, 0x78, 0x3f, 0xef, 0x6, 0xf0, 0x7b, 0xfc,

    /* U+005B "[" */
    0xf8, 0xf0, 0x7f, 0xf5, 0xbc,

    /* U+005C "\\" */
    0xc3, 0xff, 0x85, 0xe0, 0xff, 0xe1, 0x78, 0x3f,
    0xbc, 0x1f, 0xfc, 0x2f, 0x7, 0xf8,

    /* U+005D "]" */
    0xfe, 0xf, 0xfe, 0xb7, 0x80,

    /* U+005E "^" */
    0x6, 0x17, 0xc7, 0x8f, 0x0,

    /* U+005F "_" */
    0xff, 0x0,

    /* U+0060 "`" */
    0xc1, 0xe0, 0x78,

    /* U+0061 "a" */
    0x3f, 0x3, 0xf8, 0x3e, 0xfc, 0x7f, 0x7, 0xff,
    0x7, 0xf8,

    /* U+0062 "b" */
    0xc3, 0xff, 0x8d, 0xf8, 0x1f, 0xc1, 0xff, 0xd3,
    0xfe,

    /* U+0063 "c" */
    0x3f, 0x1f, 0xe0, 0xec, 0x3f, 0xf9, 0x7f, 0xf0,

    /* U+0064 "d" */
    0xe, 0xc3, 0xff, 0x83, 0xf8, 0xfe, 0xf, 0xfe,
    0x9f, 0xf0,

    /* U+0065 "e" */
    0x3f, 0x1f, 0xe0, 0xfb, 0xf0, 0x3f, 0x83, 0xfe,
    0xff, 0x80,

    /* U+0066 "f" */
    0xb, 0xe0, 0x7e, 0xf, 0x78, 0xfe, 0x3e, 0xf,
    0xfe, 0xc0,

    /* U+0067 "g" */
    0x3f, 0xfc, 0x1f, 0xfd, 0x3f, 0xe0, 0x7e, 0xf,
    0xdf, 0xc0,

    /* U+0068 "h" */
    0xc3, 0xff, 0x8f, 0xf0, 0x3f, 0x8c, 0x3f, 0xfa,
    0xc0,

    /* U+0069 "i" */
    0x30, 0x61, 0xbc, 0x61, 0xff, 0xc5, 0xcc,

    /* U+006A "j" */
    0x6, 0xc, 0x2f, 0xe0, 0xff, 0xe6, 0xfc,

    /* U+006B "k" */
    0xc3, 0xff, 0x95, 0x85, 0xf0, 0x3e, 0x7, 0x83,
    0x78, 0x3b, 0xc1, 0xde, 0xe, 0xf0,

    /* U+006C "l" */
    0xf1, 0x87, 0xff, 0x43,

    /* U+006D "m" */
    0xcf, 0x8f, 0xc0, 0xff, 0xf1, 0x87, 0xff, 0xd0,

    /* U+006E "n" */
    0xcf, 0x81, 0xfc, 0x61, 0xff, 0xd6,

    /* U+006F "o" */
    0x3f, 0x1f, 0xe0, 0xff, 0xe9, 0x7f, 0x80,

    /* U+0070 "p" */
    0xfe, 0x7, 0xf0, 0x7f, 0xf4, 0xff, 0x8f, 0xc1,
    0xff, 0xc1,

    /* U+0071 "q" */
    0x3f, 0xfc, 0x1f, 0xfd, 0x3f, 0xe0, 0x7e, 0xf,
    0xfe, 0x8,

    /* U+0072 "r" */
    0xcf, 0x8f, 0xc6, 0x1f, 0xfd, 0x0,

    /* U+0073 "s" */
    0x3f, 0x1f, 0xe0, 0xef, 0xc1, 0xbf, 0x6, 0xfc,
    0x1d, 0xfe,

    /* U+0074 "t" */
    0x6, 0x1f, 0xfc, 0x3f, 0x1f, 0xc7, 0xc1, 0xff,
    0xd4, 0xfc,

    /* U+0075 "u" */
    0xc3, 0x61, 0xff, 0xd6, 0xcf, 0xe0,

    /* U+0076 "v" */
    0xc3, 0xb0, 0xfb, 0xc1, 0x78, 0x3f, 0x78, 0xf0,
    0x7f, 0xbe, 0xf, 0xe0,

    /* U+0077 "w" */
    0xc3, 0x61, 0xb0, 0xff, 0xe1, 0xfc, 0x1b, 0xc1,
    0xf7, 0x82, 0xf1, 0xe0, 0xff, 0xe1, 0xfc, 0x17,
    0xc1, 0xff, 0xc0,

    /* U+0078 "x" */
    0xc3, 0xbe, 0xb, 0xc7, 0x8f, 0x5, 0xf0, 0x7f,
    0xdf, 0x5, 0xe3, 0xc7, 0x82, 0xf0,

    /* U+0079 "y" */
    0xc3, 0xb0, 0xfb, 0xc1, 0x78, 0x3f, 0xf8, 0x7e,
    0x3c, 0x1f, 0xef, 0x83, 0xfe, 0xf0, 0x5f, 0x6,

    /* U+007A "z" */
    0xff, 0xf8, 0x3b, 0xc1, 0x78, 0x2f, 0x5, 0xe0,
    0xbc, 0x1d, 0xfc,

    /* U+007B "{" */
    0x7, 0x8f, 0x83, 0xff, 0x89, 0xe0, 0x78, 0x3f,
    0xf9, 0x7f, 0x0,

    /* U+007C "|" */
    0xc3, 0xff, 0x82,

    /* U+007D "}" */
    0xf0, 0x3e, 0xf, 0xfe, 0x3f, 0x81, 0xe0, 0xff,
    0xe4, 0x7c, 0x0,

    /* U+007E "~" */
    0x3c, 0x1b, 0xf0, 0x3c, 0xf, 0xc0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 112, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 0, .adv_w = 96, .box_w = 3, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 7, .adv_w = 96, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 8},
    {.bitmap_index = 9, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 23, .adv_w = 128, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 44, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 61, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 77, .adv_w = 64, .box_w = 1, .box_h = 3, .ofs_x = 1, .ofs_y = 8},
    {.bitmap_index = 78, .adv_w = 96, .box_w = 3, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 86, .adv_w = 96, .box_w = 3, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 94, .adv_w = 96, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 101, .adv_w = 96, .box_w = 5, .box_h = 5, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 107, .adv_w = 80, .box_w = 2, .box_h = 4, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 110, .adv_w = 96, .box_w = 5, .box_h = 1, .ofs_x = 0, .ofs_y = 5},
    {.bitmap_index = 111, .adv_w = 80, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 113, .adv_w = 96, .box_w = 5, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 129, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 142, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 152, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 166, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 179, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 193, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 206, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 219, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 229, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 241, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 254, .adv_w = 80, .box_w = 2, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 259, .adv_w = 80, .box_w = 2, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 265, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 273, .adv_w = 96, .box_w = 5, .box_h = 3, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 276, .adv_w = 80, .box_w = 4, .box_h = 7, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 284, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 297, .adv_w = 144, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 315, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 331, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 349, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 360, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 370, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 382, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 393, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 407, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 417, .adv_w = 64, .box_w = 3, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 422, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 430, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 448, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 452, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 476, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 493, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 505, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 516, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 529, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 544, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 560, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 566, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 574, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 590, .adv_w = 192, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 611, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 629, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 642, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 658, .adv_w = 96, .box_w = 3, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 663, .adv_w = 96, .box_w = 5, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 677, .adv_w = 96, .box_w = 3, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 682, .adv_w = 96, .box_w = 5, .box_h = 3, .ofs_x = 0, .ofs_y = 8},
    {.bitmap_index = 687, .adv_w = 112, .box_w = 6, .box_h = 1, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 689, .adv_w = 96, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 8},
    {.bitmap_index = 692, .adv_w = 112, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 702, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 711, .adv_w = 112, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 719, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 729, .adv_w = 112, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 739, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 749, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 759, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 768, .adv_w = 64, .box_w = 3, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 775, .adv_w = 64, .box_w = 3, .box_h = 14, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 782, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 796, .adv_w = 64, .box_w = 3, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 800, .adv_w = 192, .box_w = 11, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 808, .adv_w = 112, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 814, .adv_w = 112, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 821, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 831, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 841, .adv_w = 96, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 847, .adv_w = 112, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 857, .adv_w = 112, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 867, .adv_w = 112, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 873, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 885, .adv_w = 192, .box_w = 11, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 904, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 918, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 934, .adv_w = 112, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 945, .adv_w = 112, .box_w = 4, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 956, .adv_w = 64, .box_w = 1, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 959, .adv_w = 112, .box_w = 4, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 970, .adv_w = 128, .box_w = 7, .box_h = 3, .ofs_x = 0, .ofs_y = 4}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 2,
    .kern_classes = 0,
    .bitmap_format = 1,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t ui_font_Ark16 = {
#else
lv_font_t ui_font_Ark16 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 15,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = 0,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if UI_FONT_ARK16*/

