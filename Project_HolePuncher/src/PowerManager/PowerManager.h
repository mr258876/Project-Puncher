#ifndef _POWERMANAGER_H_
#define _POWERMANAGER_H_

#include <stdint.h>
#include "PCF8574.h"
#include "PuncherSemaphore.h"

enum
{
    PM_VOLTAGE_5V = 0,
    PM_VOLTAGE_9V,
    PM_VOLTAGE_12V,
    PM_VOLTAGE_15V,
    PM_VOLTAGE_20V,
    PM_VOLTAGE_NONE = 255,
};
typedef uint8_t pm_votage_t;

class PowerManager
{
private:
    uint8_t i2c_scl;
    uint8_t i2c_sda;
    uint8_t int_pin;

    int timeout = 5000;

    PCF8574 *io;
    EventGroupHandle_t eg;
    std::function<void(uint8_t)> cb = NULL;

    friend void pm_isr(void *pvParameters);
    friend void TaskProcessPGood(void *pvParameters);

public:
    PowerManager(uint8_t i2c_scl, uint8_t i2c_sda, uint8_t int_pin);
    ~PowerManager();

    void begin();
    void acquire_voltage(uint8_t votage);
    void set_pgood_cb(std::function<void(uint8_t)> cb);
    inline void set_timeout(int timeout) { this->timeout = timeout; }
};

#endif