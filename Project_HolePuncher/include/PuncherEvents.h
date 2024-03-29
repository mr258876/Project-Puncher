#ifndef _PUNCHER_EVENTS_H_
#define _PUNCHER_EVENTS_H_

#include <any>

enum puncher_event_code_t
{
    PUNCHER_EVENT_ERROR = -1,
    PUNCHER_EVENT_SUCCESS = 0,

    PUNCHER_EVENT_SETTING_VALUE_CHANGED = 100,

    PUNCHER_EVENT_MISSION_START = 200,
    PUNCHER_EVENT_MISSION_PAUSE = 201,
    PUNCHER_EVENT_MISSION_END = 202,
    PUNCHER_EVENT_MISSION_TRANSMIT_START = 203,
    PUNCHER_EVENT_MISSION_TRANSMIT_END = 204,

    PUNCHER_EVENT_SERIAL_CONNECTED = 300,
    PUNCHER_EVENT_SERIAL_DISCONNECTED = 301,
    PUNCHER_EVENT_WIFI_CONNECTED = 302,
    PUNCHER_EVENT_WIFI_DISCONNECTED = 303,

    PUNCHER_ERROR_INVALID_OPERATION = 500,
    PUNCHER_ERROR_MOTOR_STUCK = 501,
    PUNCHER_ERROR_SLIP_TAPE = 502,

};

struct puncher_event_t
{
    puncher_event_t(puncher_event_code_t c, std::any d) : code(c), data(d) {}
    puncher_event_code_t code;
    std::any data;
};

struct puncher_event_setting_change_t
{
    puncher_event_setting_change_t(const char *n, std::any d) : item_name(n), data(d) {}
    const char *item_name;
    std::any data;
};

#endif // _PUNCHER_EVENTS_H_