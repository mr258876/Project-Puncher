#include "AS5600PositionSensor.h"

void as5600loop(void *param)
{
    AS5600PositionSensor *ps = (AS5600PositionSensor *)param;
    while (1)
    {
        ps->getAngle();
        vTaskDelay(pdMS_TO_TICKS(1));
    }
    vTaskDelete(NULL);
}

AS5600PositionSensor::AS5600PositionSensor(TwoWire *i2c_port, SemaphoreHandle_t *mutex)
{
    sensor = new AS5600(i2c_port);
    port_mutex = mutex;
}

AS5600PositionSensor::~AS5600PositionSensor()
{
}

void AS5600PositionSensor::begin()
{
    self_mutex = xSemaphoreCreateMutex();

    AS5600_Config conf;

    conf.fastFilterThreshold = 0b011;
    conf.hysteresis = 0b01;
    conf.outputStage = 0b00;
    conf.powerMode = 0b00;
    conf.pwmFrequency = 0b00;
    conf.slowFilter = 0b10;
    conf.watchdog = 0b0;

    xSemaphoreTake(*port_mutex, portMAX_DELAY);
    {
        sensor->setConfig(conf);
    }
    xSemaphoreGive(*port_mutex);

    getAngle();
    clearRelativePosition();

    xTaskCreate(as5600loop, "AS5600 Loop", 4096, this, 2, NULL);
}

/* in RAD */
double AS5600PositionSensor::getAngle()
{
    xSemaphoreTake(self_mutex, portMAX_DELAY);

    int raw = 0;
    xSemaphoreTake(*port_mutex, portMAX_DELAY);
    {
        raw = 4095 - sensor->getRawAngle();
    }
    xSemaphoreGive(*port_mutex);

    double res = raw * 1.0 * 2 * 3.14159265358979 / 4096;

    // update relative pos
    int raw_overflow = 0;
    if (raw < 1024 && last_raw_angle >= 3072)
    {
        raw_overflow = raw + 4096;
    }
    else if (raw >= 3072 && last_raw_angle < 1024)
    {
        raw_overflow = raw - 4096;
    }
    else
    {
        raw_overflow = raw;
    }

    raw_relative_pos += (raw_overflow - last_raw_angle);

    last_raw_angle = raw;

    xSemaphoreGive(self_mutex);
    return res;
}

/* set current position to zero */
void AS5600PositionSensor::clearRelativePosition()
{
    xSemaphoreTake(self_mutex, portMAX_DELAY);
    {
        raw_relative_pos = 0;
    }
    xSemaphoreGive(self_mutex);
}

/* degree since last setZero() */
double AS5600PositionSensor::getRelativePosition()
{
    double res = 0;

    xSemaphoreTake(self_mutex, portMAX_DELAY);
    {
        res = raw_relative_pos * 1.0 * 2 * 3.14159265358979 / 4096;
    }
    xSemaphoreGive(self_mutex);

    return res;
}

position_res_t AS5600PositionSensor::ping()
{
    AS5600_Status status;
    xSemaphoreTake(*port_mutex, portMAX_DELAY);
    {
        status = sensor->getStatus();
    }
    xSemaphoreGive(*port_mutex);
    ESP_LOGI("AS5600", "Magnet too weak: %d", status.magnetLow);
    ESP_LOGI("AS5600", "Magnet too strong: %d", status.magnetHigh);
    ESP_LOGI("AS5600", "Magnet detected: %d", status.magnetDetected);
    return (status.magnetDetected && !(status.magnetHigh || status.magnetLow)) ? POSITION_RES_SUCCESS : POSITION_RES_FAIL;
}