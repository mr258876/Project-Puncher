#ifndef _PUNCHERSCHEDULERTYPING_H_
#define _PUNCHERSCHEDULERTYPING_H_

#include <sys/cdefs.h>
#include <stdint.h>

struct __packed scheduler_hole_t
{
    int x;
    double z;
};

struct __packed puncher_basic_status_t
{
    uint8_t has_mission : 1;
    uint8_t is_running : 1;
    uint8_t is_feeding_paper : 1;
    uint8_t is_transmitting : 1;
    uint8_t has_error : 1;
};


struct __packed puncher_connectivity_status_t
{
    uint8_t serial_connected : 1;
    uint8_t wifi_connected : 1;
};


struct __packed puncher_status_t
{
    // basic status
    union puncher_basic_status_u
    {
        puncher_basic_status_t status_flags;
        uint32_t status_data;
    };
    puncher_basic_status_u basic_status;

    // connectivity
    union puncher_connectivity_status_u
    {
        puncher_connectivity_status_t status_flags;
        uint32_t status_data;
    };
    puncher_connectivity_status_u connectivity_status;
};

inline int puncher_is_busy(puncher_status_t &status)
{
    return status.basic_status.status_data & ~(0b1 << 31);
}

#endif // _PUNCHERSCHEDULERTYPING_H_