#include "./lv_i18n.h"


////////////////////////////////////////////////////////////////////////////////
// Define plural operands
// http://unicode.org/reports/tr35/tr35-numbers.html#Operands

// Integer version, simplified

#define UNUSED(x) (void)(x)

static inline uint32_t op_n(int32_t val) { return (uint32_t)(val < 0 ? -val : val); }
static inline uint32_t op_i(uint32_t val) { return val; }
// always zero, when decimal part not exists.
static inline uint32_t op_v(uint32_t val) { UNUSED(val); return 0;}
static inline uint32_t op_w(uint32_t val) { UNUSED(val); return 0; }
static inline uint32_t op_f(uint32_t val) { UNUSED(val); return 0; }
static inline uint32_t op_t(uint32_t val) { UNUSED(val); return 0; }

static lv_i18n_phrase_t en_us_singulars[] = {
    {"Operational Speed (mm/s)", "Operational\nSpeed (mm/s)"},
    {"_length_type_options", "Diameter\nPerimeter"},
    {"Operational Current (mA)", "Operational\nCurrent (mA)"},
    {"_idle_behavior_options", "Keep Current\nReduce Current\nCut Power"},
    {"Zeroing Torch Threshold", "Zeroing Torch\nThreshold"},
    {"Zeroing Speed (mm/s)", "Zeroing Speed\n(mm/s)"},
    {"Zeroing Position (mm)", "Zeroing Position\n(mm)"},
    {"_motor_voltage_options", "5V\n9V\n12V\n15V\n20V"},
    {"_language_options", "English\n简体中文"},
    {"Measured Residual (mm)", "Measured\nResidual (mm)"},
    {"Punch Depth (mm)", "Punch Depth\n(mm)"},
    {NULL, NULL} // End mark
};



static uint8_t en_us_plural_fn(int32_t num)
{
    uint32_t n = op_n(num); UNUSED(n);
    uint32_t i = op_i(n); UNUSED(i);
    uint32_t v = op_v(n); UNUSED(v);

    if ((i == 1 && v == 0)) return LV_I18N_PLURAL_TYPE_ONE;
    return LV_I18N_PLURAL_TYPE_OTHER;
}

static const lv_i18n_lang_t en_us_lang = {
    .locale_name = "en-US",
    .singulars = en_us_singulars,

    .locale_plural_fn = en_us_plural_fn
};

static lv_i18n_phrase_t zh_cn_singulars[] = {
    {"Status", "状态"},
    {"ETA", "预计剩余时间"},
    {"Feed", "进纸"},
    {"Setting", "设置"},
    {"Info", "信息"},
    {"Warning", "警告"},
    {"Error", "错误"},
    {"Back", "返回"},
    {"About", "关于"},
    {"STOP", "停止"},
    {"Connectivity", "连接"},
    {"Motors", "电机"},
    {"Display", "显示"},
    {"Disp.", "显示"},
    {"Others", "其他"},
    {"Brightness", "亮度"},
    {"BLE", "蓝牙"},
    {"X Axis", "X轴"},
    {"Y Axis", "Y轴"},
    {"Z Axis", "Z轴"},
    {"Mechanical", "机械"},
    {"Lead Length (mm)", "导程长度(mm)"},
    {"Operational Speed (mm/s)", "运行速度(mm/s)"},
    {"Length Type", "长度类型"},
    {"_length_type_options", "直径\n周长"},
    {"Reverse Axis", "反转方向"},
    {"Electrical", "电气"},
    {"Operational Current (mA)", "运行电流(mA)"},
    {"Idle Behavior", "空闲行为"},
    {"_idle_behavior_options", "保持电流\n减小电流\n切断电源"},
    {"Zeroing", "归位"},
    {"Auto Zeroing", "自动归位"},
    {"Zeroing Torch Threshold", "归位力矩阈值"},
    {"Zeroing Current (mA)", "归位电流(mA)"},
    {"Zeroing Speed (mm/s)", "归位速度(mm/s)"},
    {"Zeroing Position (mm)", "归位后位置(mm)"},
    {"Power", "供电"},
    {"Voltage", "电压"},
    {"Motor_Voltage", "电机电压"},
    {"Screen", "屏幕"},
    {"System", "系统"},
    {"Language", "语言"},
    {"_language_options", "English\n简体中文"},
    {"Mcode Tickrate", "Mcode 每小节tick数"},
    {"Calibration", "校准"},
    {"Target Bar", "目标小节数"},
    {"Measured Bar", "测量小节数"},
    {"Measured Residual (mm)", "测量残差(mm)"},
    {"Punch Depth (mm)", "打孔深度(mm)"},
    {NULL, NULL} // End mark
};



static uint8_t zh_cn_plural_fn(int32_t num)
{



    return LV_I18N_PLURAL_TYPE_OTHER;
}

static const lv_i18n_lang_t zh_cn_lang = {
    .locale_name = "zh-CN",
    .singulars = zh_cn_singulars,

    .locale_plural_fn = zh_cn_plural_fn
};

const lv_i18n_language_pack_t lv_i18n_language_pack[] = {
    &en_us_lang,
    &zh_cn_lang,
    NULL // End mark
};

////////////////////////////////////////////////////////////////////////////////


// Internal state
static const lv_i18n_language_pack_t * current_lang_pack;
static const lv_i18n_lang_t * current_lang;


/**
 * Reset internal state. For testing.
 */
void __lv_i18n_reset(void)
{
    current_lang_pack = NULL;
    current_lang = NULL;
}

/**
 * Set the languages for internationalization
 * @param langs pointer to the array of languages. (Last element has to be `NULL`)
 */
int lv_i18n_init(const lv_i18n_language_pack_t * langs)
{
    if(langs == NULL) return -1;
    if(langs[0] == NULL) return -1;

    current_lang_pack = langs;
    current_lang = langs[0];     /*Automatically select the first language*/
    return 0;
}

/**
 * Change the localization (language)
 * @param l_name name of the translation locale to use. E.g. "en-GB"
 */
int lv_i18n_set_locale(const char * l_name)
{
    if(current_lang_pack == NULL) return -1;

    uint16_t i;

    for(i = 0; current_lang_pack[i] != NULL; i++) {
        // Found -> finish
        if(strcmp(current_lang_pack[i]->locale_name, l_name) == 0) {
            current_lang = current_lang_pack[i];
            return 0;
        }
    }

    return -1;
}


static const char * __lv_i18n_get_text_core(lv_i18n_phrase_t * trans, const char * msg_id)
{
    uint16_t i;
    for(i = 0; trans[i].msg_id != NULL; i++) {
        if(strcmp(trans[i].msg_id, msg_id) == 0) {
            /*The msg_id has found. Check the translation*/
            if(trans[i].translation) return trans[i].translation;
        }
    }

    return NULL;
}


/**
 * Get the translation from a message ID
 * @param msg_id message ID
 * @return the translation of `msg_id` on the set local
 */
const char * lv_i18n_get_text(const char * msg_id)
{
    if(current_lang == NULL) return msg_id;

    const lv_i18n_lang_t * lang = current_lang;
    const void * txt;

    // Search in current locale
    if(lang->singulars != NULL) {
        txt = __lv_i18n_get_text_core(lang->singulars, msg_id);
        if (txt != NULL) return txt;
    }

    // Try to fallback
    if(lang == current_lang_pack[0]) return msg_id;
    lang = current_lang_pack[0];

    // Repeat search for default locale
    if(lang->singulars != NULL) {
        txt = __lv_i18n_get_text_core(lang->singulars, msg_id);
        if (txt != NULL) return txt;
    }

    return msg_id;
}

/**
 * Get the translation from a message ID and apply the language's plural rule to get correct form
 * @param msg_id message ID
 * @param num an integer to select the correct plural form
 * @return the translation of `msg_id` on the set local
 */
const char * lv_i18n_get_text_plural(const char * msg_id, int32_t num)
{
    if(current_lang == NULL) return msg_id;

    const lv_i18n_lang_t * lang = current_lang;
    const void * txt;
    lv_i18n_plural_type_t ptype;

    // Search in current locale
    if(lang->locale_plural_fn != NULL) {
        ptype = lang->locale_plural_fn(num);

        if(lang->plurals[ptype] != NULL) {
            txt = __lv_i18n_get_text_core(lang->plurals[ptype], msg_id);
            if (txt != NULL) return txt;
        }
    }

    // Try to fallback
    if(lang == current_lang_pack[0]) return msg_id;
    lang = current_lang_pack[0];

    // Repeat search for default locale
    if(lang->locale_plural_fn != NULL) {
        ptype = lang->locale_plural_fn(num);

        if(lang->plurals[ptype] != NULL) {
            txt = __lv_i18n_get_text_core(lang->plurals[ptype], msg_id);
            if (txt != NULL) return txt;
        }
    }

    return msg_id;
}

/**
 * Get the name of the currently used locale.
 * @return name of the currently used locale. E.g. "en-GB"
 */
const char * lv_i18n_get_current_locale(void)
{
    if(!current_lang) return NULL;
    return current_lang->locale_name;
}
