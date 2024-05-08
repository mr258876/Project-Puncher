#include "AS5600PositionSensor.h"
#include <vector>

static TaskHandle_t loop_tsk = nullptr;
static std::vector<AS5600PositionSensor *> sensor_list;

void as5600loop(void *param)
{
    std::vector<AS5600PositionSensor *> *p_sl = (std::vector<AS5600PositionSensor *> *)param;
    while (1)
    {
        for (size_t i = 0; i < p_sl->size(); i++)
        {
            p_sl->at(i)->_getAngle();
        }
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

    if (!loop_tsk)
    {
        assert(xTaskCreate(as5600loop, "AS5600 Loop", 4096, &sensor_list, 2, &loop_tsk) == pdPASS);
    }
    sensor_list.push_back(this);
}

/* in RAD */
double AS5600PositionSensor::_getAngle()
{
    xSemaphoreTake(self_mutex, portMAX_DELAY);

    int raw = 0;
    xSemaphoreTake(*port_mutex, portMAX_DELAY);
    {
        raw = 4095 - sensor->getRawAngle();
    }
    xSemaphoreGive(*port_mutex);

    if (use_cali_profile)
    {
        uint16_t val1 = cali_profile[raw / 8];
        uint16_t val2 = (raw / 8 < 511) ? cali_profile[raw / 8 + 1] : cali_profile[0];
        if (val1 > 3072 && val2 < 1024)
            val2 += 4096;
        raw = val1 + (val2 - val1) * 1.0 / 8 * (raw % 8);

        if (raw > 4095)
            raw -= 4096;
    }

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

/* load calibration profile */
position_res_t AS5600PositionSensor::loadCalibrationProfile(void *profile)
{
    if (profile)
    {
        memcpy(cali_profile, profile, sizeof(cali_profile));
        use_cali_profile = true;
    }
    else
    {
        use_cali_profile = false;
    }

    return POSITION_RES_SUCCESS;
}

size_t AS5600PositionSensor::getCalibrationProfileSize()
{
    return sizeof(cali_profile);
}

struct AS5600CaliStruct
{
    AS5600PositionSensor *sensor;
    EventGroupHandle_t evt_group;
    MotorController *axis;
    long process;
    long steps_to_move;
};

void tsk_axis_cali(void *param)
{
    AS5600CaliStruct *cali_str = (AS5600CaliStruct *)param;
    xSemaphoreTake(cali_str->sensor->self_mutex, portMAX_DELAY);
    vTaskSuspend(loop_tsk);
    xSemaphoreGive(cali_str->sensor->self_mutex);
    int16_t *reading_CW = (int16_t *)malloc(sizeof(int16_t) * 512);
    int16_t reading;
    cali_str->sensor->use_cali_profile = false;
    while (1)
    {
        xEventGroupWaitBits(cali_str->evt_group, 1, pdTRUE, pdTRUE, portMAX_DELAY);

        cali_str->process += 1;
        cali_str->sensor->_getAngle();
        reading = cali_str->sensor->rawReading();

        // ESP_LOGI("AS5600", "%ld %d", cali_str->process, reading);
        if (cali_str->process > 511)
        {
            break;
        }
        else
        {
            reading_CW[cali_str->process] = reading;

            cali_str->axis->move(cali_str->steps_to_move);
            vTaskDelay(pdMS_TO_TICKS(2));
        }
    }

    int16_t start_pos = reading_CW[0];
    int16_t start_pos_mod = start_pos % 8;
    int16_t start_pos_int = start_pos / 8;
    int16_t curr_offset = start_pos_int; // offset of lookup table (index is measured values)
    if (start_pos_mod)
        curr_offset += 1; // start with the next offset if start_pos mod 8 > 0 (e.g. if start_pos is 5, we start from 8)
    int16_t finished = 0;
    for (int i = 0; i < 512; i++) // Fit correction curve
    {
        int16_t reading_i = reading_CW[i];
        int16_t reading_i_p1 = (i < 511) ? reading_CW[i + 1] : reading_CW[0];

        if (reading_i > 3072 && reading_i_p1 < 1024)
        {
            reading_i_p1 += 4096;
        }
        else if (reading_i < 1024 && reading_i_p1 > 3072)
        {
            reading_i_p1 -= 4096;
        }

        while (1)
        {
            if (reading_i < curr_offset * 8 && curr_offset * 8 < reading_i_p1)
            {
                // Expected reading[i] + (expected reading change after each move, here is 8) / (measurement[i+1] - measurement[i]) * (measurement in table - measurement[i])
                int16_t exp_val = ((start_pos + i * 8 > 4095) ? (start_pos + i * 8 - 4096) : (start_pos + i * 8)) + (8.0 / (reading_i_p1 - reading_i) * (curr_offset * 8 - reading_i));
                if (exp_val > 4095)
                {
                    exp_val -= 4096;
                }
                else if (exp_val < 0)
                {
                    exp_val += 4096;
                }

                if (curr_offset > 511)
                    curr_offset -= 512;

                cali_str->sensor->cali_profile[curr_offset] = exp_val;
                curr_offset += 1;
                finished += 1;
            }
            else if (reading_i == curr_offset * 8)
            {
                if (curr_offset > 511)
                    curr_offset -= 512;

                cali_str->sensor->cali_profile[curr_offset] = (start_pos + i * 8 > 4095) ? (start_pos + i * 8 - 4096) : (start_pos + i * 8);
                curr_offset += 1;
                finished += 1;
            }
            else
            {
                break;
            }
        }
    }
    ESP_LOGI("AS5600", "%d %d %d %d %d", start_pos, start_pos_mod, start_pos_int, curr_offset, finished);
    cali_str->sensor->use_cali_profile = true;

    if (cali_str->sensor->cal_fn_cb)
    {
        cali_str->sensor->cal_fn_cb();
    }

    vTaskResume(loop_tsk);
    vEventGroupDelete(cali_str->evt_group);
    free(cali_str);
    vTaskDelete(NULL);
}

static void IRAM_ATTR axis_cali_move_cb(void *param)
{
    AS5600CaliStruct *cali_str = (AS5600CaliStruct *)param;
    BaseType_t pxHigherPriorityTaskWoken;
    xEventGroupSetBitsFromISR(cali_str->evt_group, 1, &pxHigherPriorityTaskWoken);
}

position_res_t AS5600PositionSensor::startCalibration(MotorController *axis, uint32_t microsteps_per_revolution)
{
    AS5600CaliStruct *cali_str = (AS5600CaliStruct *)malloc(sizeof(AS5600CaliStruct));
    cali_str->sensor = this;
    cali_str->axis = axis;
    cali_str->process = -1;
    cali_str->evt_group = xEventGroupCreate();
    cali_str->steps_to_move = microsteps_per_revolution / 512;

    assert(xTaskCreate(tsk_axis_cali, "tskAxisCali", 4096, cali_str, 2, NULL) == pdPASS);
    axis->setMoveFinishCallBack(axis_cali_move_cb, cali_str);
    axis->move(cali_str->steps_to_move);

    return POSITION_RES_SUCCESS;
}

position_res_t AS5600PositionSensor::setCalibrationFinishCallBack(std::function<void()> cb)
{
    this->cal_fn_cb = cb;
    return POSITION_RES_SUCCESS;
}