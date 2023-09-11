#ifndef _PUNCHERSCHEDULERTYPING_H_
#define _PUNCHERSCHEDULERTYPING_H_

#include <sys/cdefs.h>
#include <stdint.h>

struct __packed scheduler_hole_t
{
    int x;
    double z;
};

struct __packed puncher_status_t
{
    // basic status
    union u_basic_status
    {
        struct basic_status_flags
        {
            uint8_t has_mission : 1;
            uint8_t is_running : 1;
            uint8_t is_transmitting : 1;
            uint8_t has_error : 1;
        };
        unsigned status_data;
    };
    u_basic_status basic_status;

    // connectivity
    union u_connectivity_status
    {
        struct connectivity_status_flags
        {
            uint8_t serial_connected : 1;
            uint8_t wifi_connected : 1;
        };
        unsigned status_data;
    };
    u_connectivity_status connectivity_status;
};

// struct scheduler_request_t
// {
//     scheduler_rquest_endpoint_t endpoint;
//     void *request_data;
//     void *response_data;
// };


#endif  // _PUNCHERSCHEDULERTYPING_H_