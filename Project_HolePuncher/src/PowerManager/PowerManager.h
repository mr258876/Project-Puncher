#ifndef _POWERMANAGER_H_
#define _POWERMANAGER_H_

#include <stdint.h>

enum {
    PM_VOLTAGE_5V,
    PM_VOLTAGE_9V,
    PM_VOLTAGE_12V,
    PM_VOLTAGE_15V,
    PM_VOLTAGE_20V
};
typedef uint8_t pm_votage_t;

enum {
    PM_STATUS_POWER_GOOD,
    PM_STATUS_POWER_BAD
};
typedef uint8_t pm_power_status_t;

typedef void (*pm_pgood_cb_t)(pm_power_status_t status);

void pm_init();
void pm_acquire_voltage(pm_votage_t votage);
void pm_set_pgood_cb(pm_pgood_cb_t event_cb);


#endif