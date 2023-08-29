#ifndef _PUNCHERSCHEDULERTYPING_H_
#define _PUNCHERSCHEDULERTYPING_H_

#include <sys/cdefs.h>
#include <stdint.h>

struct __packed scheduler_hole_t
{
    int x;
    double z;
};

enum puncher_status_t : uint8_t
{
    PUNCHER_IDLE = 0,
    PUNCHER_RUNNING = 0b1,
    PUNCHER_DATA_TRANSMITTING = 0b10,
    PUNCHER_PEEDING_PAPER = 0B100,
    PUNCHER_ERROR = 128,
};

enum scheduler_rquest_endpoint_t : uint8_t
{
    SCHEDULER_REQUEST_NO_TYPE,
    SCHEDULER_REQUEST_SELF_CALIBRATION,
    SCHEDULER_REQUEST_SET_STATUS,
};

struct scheduler_request_t
{
    scheduler_rquest_endpoint_t endpoint;
    void *request_data;
    void *response_data;
};


#endif  // _PUNCHERSCHEDULERTYPING_H_