#ifndef _CONFIGURABLE_H_
#define _CONFIGURABLE_H_

#include <string>
#include <vector>
#include <functional>

enum ConfigurableItem_t : uint8_t
{
    CONFIGURABLE_NO_TYPE,
    CONFIGURABLE_DUMBTEXT,
    CONFIGURABLE_CALLBACK,
    CONFIGURABLE_ENUM,
    CONFIGURABLE_SUBMENU,
    CONFIGURABLE_BOOLEAN,
    CONFIGURABLE_INTEGER,
    CONFIGURABLE_FLOAT,
};

enum ConfigurableBooleanType_t : uint8_t
{
    CONFIGURABLE_BOOLEAN_UNDEFINED,
    CONFIGURABLE_BOOLEAN_SWITCH,
    CONFIGURABLE_BOOLEAN_CHECKBOX,
};

struct __packed ConfigurableIntegerValue_t
{
    int _value;
    int _max;
    int _min;
    const char* _format;
};

struct __packed ConfigurableFloatValue_t
{
    float _value;
    float _max;
    float _min;
    const char* _format;
};

struct __packed ConfigurableItem
{
    using ItemCallBack = std::function<int(void *)>;
    const char* _name;
    ItemCallBack _callback;
    const ConfigurableItem_t _itemType = CONFIGURABLE_NO_TYPE;
};

struct __packed ConfigurableItemWithValue
{
};

struct __packed ConfigurableDumbText : ConfigurableItem, ConfigurableItemWithValue
{
    const ConfigurableItem_t _itemType = CONFIGURABLE_DUMBTEXT;
    std::string _value;

    ConfigurableDumbText(const char* name, ItemCallBack cb, std::string value)
    {
        _name = name;
        _callback = cb;
        _value = value;
    };
};

struct __packed ConfigurableCallback : ConfigurableItem, ConfigurableItemWithValue
{
    const ConfigurableItem_t _itemType = CONFIGURABLE_CALLBACK;
    const char* _value = ">>";

    ConfigurableCallback(const char* name, ItemCallBack cb)
    {
        _name = name;
        _callback = cb;
    };
};

struct __packed ConfigurableSubmenu : ConfigurableItem, ConfigurableItemWithValue
{
    const ConfigurableItem_t _itemType = CONFIGURABLE_CALLBACK;
    const char* _value = "->";

    ConfigurableSubmenu(const char* name, ItemCallBack cb)
    {
        _name = name;
        _callback = cb;
    };
};

struct __packed ConfigurableEnum : ConfigurableItem, ConfigurableItemWithValue
{
    const ConfigurableItem_t _itemType = CONFIGURABLE_ENUM;
    const char* _value;  // enum items, split with "\n"

    ConfigurableEnum(const char* name, ItemCallBack cb, const char* value)
    {
        _name = name;
        _callback = cb;
        _value = value;
    };
};

struct __packed ConfigurableBoolean : ConfigurableItem, ConfigurableItemWithValue
{
    const ConfigurableItem_t _itemType = CONFIGURABLE_BOOLEAN;
    uint8_t _value;
    ConfigurableBooleanType_t _style;  // to storage controller style, e.g. switch, checkbox

    ConfigurableBoolean(const char* name, ItemCallBack cb, uint8_t value, ConfigurableBooleanType_t style=CONFIGURABLE_BOOLEAN_CHECKBOX)
    {
        _name = name;
        _callback = cb;
        _value = value;
        _style = style;
    };
};

struct __packed ConfigurableInteger : ConfigurableItem, ConfigurableItemWithValue
{
    const ConfigurableItem_t _itemType = CONFIGURABLE_INTEGER;
    ConfigurableIntegerValue_t _value;

    ConfigurableInteger(const char* name, ItemCallBack cb, int value, int max=INT32_MIN, int min=INT32_MIN, const char* format = NULL)
    {
        _name = name;
        _callback = cb;
        _value._value = value;
        _value._max = max;
        _value._min = min;
        _value._format = format;
    };
};

struct __packed ConfigurableFloat : ConfigurableItem, ConfigurableItemWithValue
{
    const ConfigurableItem_t _itemType = CONFIGURABLE_FLOAT;
    ConfigurableFloatValue_t _value;

    ConfigurableFloat(const char* name, ItemCallBack cb, float value, float max=NULL, float min=NULL, const char* format = NULL)
    {
        _name = name;
        _callback = cb;
        _value._value = value;
        _value._max = max;
        _value._min = min;
        _value._format = format;
    };
};

class Configurable
{
private:
    const char* _conf_name;
public:
    virtual std::vector<ConfigurableItem> getConfigurations(bool full=false) = 0;
};

#endif  // _CONFIGURABLE_H_