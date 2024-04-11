#ifndef _PUNCHERSCHEDULERTYPING_H_
#define _PUNCHERSCHEDULERTYPING_H_

#include <sys/cdefs.h>
#include <stdint.h>
#include <time.h>

struct __packed scheduler_hole_t
{
    int x;
    double z;
};

struct __packed puncher_basic_status_t
{
    uint8_t has_error : 1;
    uint8_t has_mission : 1;
    uint8_t is_transmitting_data : 1;
    uint8_t is_feeding_paper : 1;
    uint8_t is_running : 1;
    uint8_t is_zeroing_x: 1;
    uint8_t is_zeroing_y: 1;
    uint8_t is_zeroing_z: 1;
    uint8_t driver_err_x: 1;
    uint8_t driver_err_y: 1;
    uint8_t driver_err_z: 1;
    uint8_t motor_err_x: 1;
    uint8_t motor_err_y: 1;
    uint8_t motor_err_z: 1;
    uint8_t sensor_err_x: 1;
    uint8_t sensor_err_y: 1;
    uint8_t sensor_err_z: 1;
    uint8_t power_err: 1;
};

#define PUNCHER_STATUS_HAS_ERROR            (1 << 0)
#define PUNCHER_STATUS_HAS_MISSION          (1 << 1)
#define PUNCHER_STATUS_IS_TRANSMITTING_DATA (1 << 2)
#define PUNCHER_STATUS_IS_FEEDING_PAPER     (1 << 3)
#define PUNCHER_STATUS_IS_RUNNING           (1 << 4)
#define PUNCHER_STATUS_IS_ZEROING_X         (1 << 5)
#define PUNCHER_STATUS_IS_ZEROING_Y         (1 << 6)
#define PUNCHER_STATUS_IS_ZEROING_Z         (1 << 7)
#define PUNCHER_STATUS_BUSY_MASK            (~0b11)
#define PUNCHER_STATUS_IS_ZEROING           (0b11100000)
#define PUNCHER_STATUS_ERROR_CHECK          (0b111111111100000000)


struct __packed puncher_connectivity_status_t
{
    uint8_t USB_connected : 1;
    uint8_t serial_connected : 1;
    uint8_t wifi_enabled : 1;
    uint8_t wifi_connected : 1;
    uint8_t bluetooth_enabled : 1;
    uint8_t bluetooth_connected : 1;
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

    // data
    uint32_t task_length;
    uint32_t finished_length;

    // ETA
    time_t ETA;
};

inline int puncher_is_busy(puncher_status_t &status)
{
    return status.basic_status.status_data & ~(0b1 << 31);
}

#endif // _PUNCHERSCHEDULERTYPING_H_