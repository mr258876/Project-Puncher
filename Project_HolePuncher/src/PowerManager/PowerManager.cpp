#include "PowerManager.h"
#include "PCF8574.h"
#include "PuncherSemaphore.h"

#define CTP_I2C_SCL 6
#define CTP_I2C_SDA 7
#define PCF8574_INT 14

#define IsrEvent (1 << 0) // 1

static EventGroupHandle_t eg;
static pm_pgood_cb_t cb = NULL;
static PCF8574 io(0x20, &Wire1);

static uint8_t get_power_io(pm_votage_t votage)
{
    switch (votage)
    {
    case PM_VOLTAGE_5V:
        return 0b11000000;
    case PM_VOLTAGE_9V:
        return 0b10000000;
    case PM_VOLTAGE_12V:
        return 0b10010000;
    case PM_VOLTAGE_15V:
        return 0b10110000;
    case PM_VOLTAGE_20V:
        return 0b10100000;
    default:
        return 0;
    }
}

static void IRAM_ATTR pm_isr()
{
    BaseType_t xHigherPriorityTaskWoken;
    xEventGroupSetBitsFromISR(eg, IsrEvent, &xHigherPriorityTaskWoken);
}

static void TaskProcessPGood(void *pvParameters)
{
    for (;;)
    {
        xEventGroupWaitBits(eg, IsrEvent, pdTRUE, pdTRUE, portMAX_DELAY);
        {
            uint8_t val = 0;
            xSemaphoreTake(I2C1Mutex, portMAX_DELAY);
            {
                val = io.read8();
                io.write8(val);
            }
            xSemaphoreGive(I2C1Mutex);

            if (cb)
            {
                cb(val >> 7);
            }
        }
    }
}

void pm_init()
{
    xSemaphoreTake(I2C1Mutex, portMAX_DELAY);
    {
        io.begin(CTP_I2C_SDA, CTP_I2C_SCL, get_power_io(PM_VOLTAGE_5V));
    }
    xSemaphoreGive(I2C1Mutex);

    eg = xEventGroupCreate();

    pinMode(PCF8574_INT, INPUT_PULLUP);
    attachInterrupt(PCF8574_INT, pm_isr, FALLING);

    xTaskCreatePinnedToCore(TaskProcessPGood, "fTaskProcessPGood", 4096, NULL, 3, NULL, 1);
}

void pm_acquire_voltage(pm_votage_t votage)
{
    xSemaphoreTake(I2C1Mutex, portMAX_DELAY);
    {
        io.write8(get_power_io(votage));
    }
    xSemaphoreGive(I2C1Mutex);
}

void pm_set_pgood_cb(pm_pgood_cb_t event_cb)
{
    cb = event_cb;
}