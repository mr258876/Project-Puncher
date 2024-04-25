#include "PowerManager.h"

#define IsrEvent (1 << 0)           // 0b1
#define PowerLevelSetEvent (1 << 1) // 0b10

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

void IRAM_ATTR pm_isr(void *pvParameters)
{
    PowerManager *pm = (PowerManager *)pvParameters;
    BaseType_t xHigherPriorityTaskWoken;
    xEventGroupSetBitsFromISR(pm->eg, IsrEvent, &xHigherPriorityTaskWoken);
}

void TaskProcessPGood(void *pvParameters)
{
    PowerManager *pm = (PowerManager *)pvParameters;
    uint32_t evt;

    for (;;)
    {
        evt = xEventGroupWaitBits(pm->eg, 0xFF, pdTRUE, pdFALSE, portMAX_DELAY);

        if (evt & PowerLevelSetEvent)
            evt = xEventGroupWaitBits(pm->eg, IsrEvent, pdTRUE, pdTRUE, pdMS_TO_TICKS(pm->timeout));

        /*
            Read the value no matter there is a falling edge,
            since the voltage could already in acquired state
         */
        uint8_t val = 0;
        xSemaphoreTake(I2C1Mutex, portMAX_DELAY);
        {
            val = pm->io->read8();
            pm->io->write8(val);
        }
        xSemaphoreGive(I2C1Mutex);

        if (pm->cb)
        {
            pm->cb(val >> 7); // 0 for power good
            ESP_LOGI("PowerManager", "readings: %d", val);
        }
    }
}

void PowerManager::begin()
{
    io = new PCF8574(0x20, &Wire1);

    xSemaphoreTake(I2C1Mutex, portMAX_DELAY);
    {
        io->begin(i2c_sda, i2c_scl, get_power_io(PM_VOLTAGE_5V));
    }
    xSemaphoreGive(I2C1Mutex);

    eg = xEventGroupCreate();

    pinMode(int_pin, INPUT_PULLUP);
    attachInterruptArg(int_pin, pm_isr, this, FALLING);

    assert(xTaskCreatePinnedToCore(TaskProcessPGood, "fTaskProcessPGood", 4096, this, 3, NULL, 1) == pdPASS);
}

void PowerManager::acquire_voltage(pm_votage_t votage)
{
    xEventGroupSetBits(eg, PowerLevelSetEvent);
    xSemaphoreTake(I2C1Mutex, portMAX_DELAY);
    {
        io->write8(get_power_io(votage));
    }
    xSemaphoreGive(I2C1Mutex);
}

void PowerManager::set_pgood_cb(std::function<void(uint8_t)> event_cb)
{
    cb = event_cb;
}

PowerManager::PowerManager(uint8_t scl, uint8_t sda, uint8_t int_p)
{
    this->i2c_scl = scl;
    this->i2c_sda = sda;
    this->int_pin = int_p;
}

PowerManager::~PowerManager()
{
}