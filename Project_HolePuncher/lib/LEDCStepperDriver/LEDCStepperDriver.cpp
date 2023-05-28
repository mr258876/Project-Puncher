#include "LEDCStepperDriver.h"

static portMUX_TYPE driver_spinlock = portMUX_INITIALIZER_UNLOCKED;

/*
 *  Helper Functions
 *  reference to pcht_ll.h & ledc_ll.h
 *
 */
static void IRAM_ATTR __pcnt_pause(uint8_t unit)
{
    PCNT.ctrl.val |= 1 << (2 * unit + 1); // Set counter pause bit
}

static void IRAM_ATTR __pcnt_resume(uint8_t unit)
{
    PCNT.ctrl.val &= ~(1 << (2 * unit + 1)); // Clear counter pause bit
}

static void IRAM_ATTR __pcnt_clear(uint8_t unit)
{
    PCNT.ctrl.val |= 1 << (2 * unit);    // Set counter rst bit
    PCNT.ctrl.val &= ~(1 << (2 * unit)); // Clear counter rst bit
}

static bool IRAM_ATTR __pcnt_is_intr_active(uint8_t unit)
{
    return ((PCNT.int_st.val) >> (unit)&1); // Get intr status bit
}

static void IRAM_ATTR __pcnt_clear_intr(uint8_t unit)
{
    PCNT.int_clr.val |= (1 << unit); // Set intr clr bit
}

static int16_t IRAM_ATTR __pcnt_get_count(uint8_t unit)
{
    return PCNT.cnt_unit[unit].cnt_val;
}

static void IRAM_ATTR __pcnt_set_threshold(uint8_t unit, int16_t value)
{
    PCNT.conf_unit[unit].conf1.cnt_thres0 = value;
}

static void IRAM_ATTR __pcnt_set_threshold_enable(uint8_t unit, uint8_t enable)
{
    PCNT.conf_unit[unit].conf0.thr_thres0_en = enable;
}

static void IRAM_ATTR __ledc_timer_pause(uint8_t mode, uint8_t timer)
{
    LEDC.timer_group[mode].timer[timer].conf.pause = 1;
}

static void IRAM_ATTR __ledc_timer_resume(uint8_t mode, uint8_t timer)
{
    LEDC.timer_group[mode].timer[timer].conf.pause = 0;
}

static void IRAM_ATTR __ledc_set_idle_level(uint8_t mode, uint8_t channel, uint8_t idle_level)
{
    LEDC.channel_group[mode].channel[channel].conf0.idle_lv = idle_level & 0x1;
}

static void IRAM_ATTR __ledc_set_sig_out_en(uint8_t mode, uint8_t channel, uint8_t sig_out_en)
{
    LEDC.channel_group[mode].channel[channel].conf0.sig_out_en = sig_out_en;
}

static void IRAM_ATTR __ledc_set_duty_start(uint8_t mode, uint8_t channel, uint8_t duty_start)
{
    LEDC.channel_group[mode].channel[channel].conf1.duty_start = duty_start;
}

static void IRAM_ATTR __ledc_set_channel_low_speed(uint8_t mode, uint8_t channel)
{
    LEDC.channel_group[mode].channel[channel].conf0.low_speed_update = 1;
}

static void IRAM_ATTR __ledc_stop(uint8_t mode, uint8_t channel, uint8_t idle_level)
{
    __ledc_set_idle_level(mode, channel, idle_level);
    __ledc_set_sig_out_en(mode, channel, false);
    __ledc_set_duty_start(mode, channel, false);
    if (mode == LEDC_LOW_SPEED_MODE)
        __ledc_set_channel_low_speed(mode, channel);
}

/*
 *
 *    LEDCStepperDriver
 *
 */
void IRAM_ATTR driver_pcnt_intr_handler(void *arg)
{
    LEDCStepperDriver *driver = (LEDCStepperDriver *)arg;
    taskENTER_CRITICAL_ISR(&driver_spinlock);
    if (__pcnt_is_intr_active(driver->pcnt_unit))
    {
        __ledc_timer_pause(driver->ledc_mode, driver->ledc_timer);
        __pcnt_pause(driver->pcnt_unit);
        __pcnt_clear_intr(driver->pcnt_unit);
        int16_t count = __pcnt_get_count(driver->pcnt_unit);
        driver->steps_remaining -= count;
        driver->step_count += count;

        if (driver->steps_remaining == 0)
        {
            __pcnt_clear(driver->pcnt_unit);
            __ledc_stop(driver->ledc_mode, driver->ledc_channel, LOW);
            driver->pcnt_running = false;
            driver->pwm_running = false;

            if (driver->auto_sleep)
            {
                if IS_CONNECTED (driver->enable_pin)
                {
                    digitalWrite(driver->enable_pin, (driver->enable_active_state == HIGH) ? LOW : HIGH);
                }
            }
        }
        else
        {
            if (driver->steps_remaining < 0)
            {
                /* Step back if over-stepped */
                driver->steps_remaining = -driver->steps_remaining;
                driver->dir_state = !driver->dir_state;
                digitalWrite(driver->dir_pin, driver->dir_state);
            }

            if (driver->steps_remaining < PCNT_THRES_VAL)
            {
                __pcnt_set_threshold(driver->pcnt_unit, driver->steps_remaining);
            }

            /* New threshold value only take effect after some specific event (e.g. zero across, overflow),
                so DO NOT clear counter before set threshold.
                See: https://www.esp32.com/viewtopic.php?t=5274 */
            __pcnt_clear(driver->pcnt_unit);
            __pcnt_resume(driver->pcnt_unit);
            __ledc_timer_resume(driver->ledc_mode, driver->ledc_timer);
        }
    }

    taskEXIT_CRITICAL_ISR(&driver_spinlock);
}

LEDCStepperDriver::LEDCStepperDriver(int motor_steps, int dir_pin, int step_pin, int enable_pin, int driver_id)
{
    if (driver_id > 7 || driver_id < 0)
    {
        driver_id = 0;
    }

    ledc_mode_t ledc_mode = (driver_id > 3 ? LEDC_HIGH_SPEED_MODE : LEDC_LOW_SPEED_MODE);
    ledc_timer_t ledc_timer = (driver_id > 3 ? static_cast<ledc_timer_t>(driver_id - 4) : static_cast<ledc_timer_t>(driver_id));
    ledc_channel_t ledc_channel = static_cast<ledc_channel_t>(driver_id);
    pcnt_unit_t pcnt_unit = static_cast<pcnt_unit_t>(driver_id);
    new (this) LEDCStepperDriver(motor_steps, dir_pin, step_pin, enable_pin, ledc_mode, ledc_timer, ledc_channel, pcnt_unit);
}

LEDCStepperDriver::LEDCStepperDriver(int motor_steps, int dir_pin, int step_pin, int enable_pin, ledc_mode_t ledc_mode, ledc_timer_t ledc_timer, ledc_channel_t ledc_channel, pcnt_unit_t pcnt_unit, pcnt_channel_t pcnt_channel, bool count_falling_edge)
{
    this->motor_steps = motor_steps;
    this->dir_pin = dir_pin;
    this->step_pin = step_pin;
    this->enable_pin = enable_pin;
    this->ledc_mode = ledc_mode;
    this->ledc_timer = ledc_timer;
    this->ledc_channel = ledc_channel;
    this->pcnt_unit = pcnt_unit;
    this->pcnt_channel = pcnt_channel;
    this->count_falling_edge = count_falling_edge;
}

LEDCStepperDriver::~LEDCStepperDriver()
{
}

void LEDCStepperDriver::driver_pwm_start()
{
    if (pwm_running)
        driver_pwm_stop();

    ESP_ERROR_CHECK(ledc_set_freq(ledc_mode, ledc_timer, pulse_freq));
    ESP_ERROR_CHECK(ledc_timer_resume(ledc_mode, ledc_timer));
    ESP_ERROR_CHECK(ledc_set_duty(ledc_mode, ledc_channel, (1 << LEDC_TIMER_4_BIT) / 2));
    ESP_ERROR_CHECK(ledc_update_duty(ledc_mode, ledc_channel));
    pwm_running = true;
}
void LEDCStepperDriver::driver_pwm_stop()
{
    ESP_ERROR_CHECK(ledc_stop(ledc_mode, ledc_channel, LOW));
    ESP_ERROR_CHECK(ledc_timer_pause(ledc_mode, ledc_timer));
    pwm_running = false;
}
void LEDCStepperDriver::driver_pcnt_start(long steps)
{
    pcnt_counter_pause(pcnt_unit);

    if (steps < PCNT_THRES_VAL)
        pcnt_set_event_value(pcnt_unit, PCNT_EVT_THRES_0, steps);
    else
        pcnt_set_event_value(pcnt_unit, PCNT_EVT_THRES_0, PCNT_THRES_VAL);

    pcnt_counter_clear(pcnt_unit);
    pcnt_counter_resume(pcnt_unit);
    pcnt_running = true;
}

void LEDCStepperDriver::begin(float rpm, short microsteps)
{
    pinMode(dir_pin, GPIO_MODE_OUTPUT);
    pinMode(step_pin, GPIO_MODE_OUTPUT);
    digitalWrite(dir_pin, dir_state);
    digitalWrite(step_pin, LOW);

    if IS_CONNECTED (enable_pin)
    {
        pinMode(enable_pin, GPIO_MODE_OUTPUT);
        disable();
    }

    //----------设置LEDC PWM----------
    ledc_timer_config_t config_ledc_timer;
    config_ledc_timer.speed_mode = ledc_mode;
    config_ledc_timer.timer_num = ledc_timer;
    config_ledc_timer.duty_resolution = LEDC_TIMER_4_BIT;
    config_ledc_timer.freq_hz = 200;
    config_ledc_timer.clk_cfg = LEDC_AUTO_CLK;
    ESP_ERROR_CHECK(ledc_timer_config(&config_ledc_timer));

    ledc_channel_config_t config_ledc_channel;
    config_ledc_channel.speed_mode = ledc_mode;
    config_ledc_channel.channel = ledc_channel;
    config_ledc_channel.timer_sel = ledc_timer;
    config_ledc_channel.intr_type = LEDC_INTR_DISABLE;
    config_ledc_channel.gpio_num = step_pin;
    ESP_ERROR_CHECK(ledc_channel_config(&config_ledc_channel));

    /* 初始化timer及channel后停止输出并暂停计数器
       使用 ledc_timer_resume(ledc_mode, ledc_timer) 及 ledc_update_duty(ledc_mode, ledc_channel) 恢复输出 */
    // ledc_stop(ledc_mode, ledc_channel, LOW);
    // ledc_timer_pause(ledc_mode, ledc_timer);
    driver_pwm_stop();
    pwm_running = false;

    //----------配置PCNT----------
    pcnt_config_t config_pcnt = {
        .pulse_gpio_num = step_pin,
        .ctrl_gpio_num = -1,                                              // 忽略控制信号   Ignore control singal
        .lctrl_mode = PCNT_MODE_KEEP,                                     // 低电平保持计数 Keep counter value if ctrl gpio low
        .hctrl_mode = PCNT_MODE_KEEP,                                     // 高电平保持计数 Keep counter value if ctrl gpio high
        .pos_mode = PCNT_COUNT_INC,                                       // 上升沿计数加一 Count up on rising edge
        .neg_mode = count_falling_edge ? PCNT_COUNT_INC : PCNT_COUNT_DIS, // 下降沿可配置计数或保持 Configurable count up or keep counter value on falling edge
        .counter_h_lim = PCNT_H_LIM_VAL,
        .counter_l_lim = PCNT_L_LIM_VAL,
        .unit = pcnt_unit,
        .channel = pcnt_channel,
    };

    ESP_ERROR_CHECK(pcnt_unit_config(&config_pcnt)); // 初始化脉冲计数器

    pcnt_set_filter_value(pcnt_unit, 100); //  忽略1.25us以内(800KHz以上)信号
    pcnt_filter_enable(pcnt_unit);

    pcnt_event_disable(pcnt_unit, PCNT_EVT_H_LIM);
    pcnt_event_disable(pcnt_unit, PCNT_EVT_L_LIM);
    pcnt_event_disable(pcnt_unit, PCNT_EVT_ZERO);
    pcnt_event_disable(pcnt_unit, PCNT_EVT_THRES_1);
    pcnt_set_event_value(pcnt_unit, PCNT_EVT_THRES_0, PCNT_THRES_VAL);
    pcnt_event_enable(pcnt_unit, PCNT_EVT_THRES_0);

    pcnt_counter_pause(pcnt_unit);
    pcnt_counter_clear(pcnt_unit);

    // pcnt_isr_service_install(ESP_INTR_FLAG_SHARED | ESP_INTR_FLAG_IRAM);
    // pcnt_isr_handler_add(pcnt_unit, driver_pcnt_intr_handler, this);
    pcnt_isr_register(driver_pcnt_intr_handler, this, ESP_INTR_FLAG_SHARED | ESP_INTR_FLAG_IRAM, &pcnt_isr_handle);
    ESP_ERROR_CHECK(pcnt_intr_enable(pcnt_unit));

    pcnt_running = false;

    /* To use LEDC and PCNT on the same pin
       See https://esp32.com/viewtopic.php?t=18115 */
    gpio_set_direction(static_cast<gpio_num_t>(step_pin), GPIO_MODE_INPUT_OUTPUT);
    gpio_matrix_out(static_cast<gpio_num_t>(step_pin), (ledc_mode == LEDC_HIGH_SPEED_MODE ? (LEDC_HS_SIG_OUT0_IDX + ledc_channel) : (LEDC_LS_SIG_OUT0_IDX + ledc_channel)), 0, 0);

    setRPM(rpm);
    setMicrostep(microsteps);
    enable();
}
/*
 * Set stepping mode (1:microsteps)
 * Allowed ranges for LEDCStepperDriver are 1:1 to 1:128
 */
short LEDCStepperDriver::setMicrostep(short microsteps)
{
    for (short ms = 1; ms <= MAX_MICROSTEP; ms <<= 1)
    {
        if (microsteps == ms)
        {
            this->microsteps = microsteps;
            break;
        }
    }
    setRPM(rpm);
    return this->microsteps;
}
void LEDCStepperDriver::setRPM(float rpm)
{
    this->rpm = rpm;
    this->pulse_freq = long(rpm * motor_steps * microsteps / 60.0);

    if (pwm_running)
        driver_pwm_start();
}
void LEDCStepperDriver::setPulseFreq(long freq)
{
    this->pulse_freq = freq;
    this->rpm = freq * 60.0 / motor_steps / microsteps;

    if (pwm_running)
        driver_pwm_start();
}
/*
 * Move the motor a given number of steps.
 * positive to move forward, negative to reverse
 */
void LEDCStepperDriver::move(long steps)
{
    startMove(steps);
    while (steps_remaining)
    {
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}
/*
 * Move the motor a given number of degrees (1-360)
 */
void LEDCStepperDriver::rotate(long deg)
{
    move(calcStepsForRotation(deg));
}
/*
 * Move the motor with sub-degree precision.
 * Note that using this function even once will add 1K to your program size
 * due to inclusion of float support.
 */
void LEDCStepperDriver::rotate(double deg)
{
    move(calcStepsForRotation(deg));
}
void LEDCStepperDriver::startMove(long steps)
{
    stop();

    if (steps < 0)
    {
        dir_state = LOW;
        steps = -steps;
    }
    else if (steps == 0)
    {
        return;
    }
    else
    {
        dir_state = HIGH;
    }

    steps_remaining = steps;
    enable();
    driver_pcnt_start(steps);
    ESP_LOGI("LEDCStepperDriver", "start move steps:%ld", steps);

    digitalWrite(dir_pin, dir_state);
    driver_pwm_start();
}
/*
 * Move the motor an integer number of degrees (360 = full rotation)
 * This has poor precision for small amounts, since step is usually 1.8deg
 */
void LEDCStepperDriver::startRotate(long deg)
{
    startMove(calcStepsForRotation(deg));
}
/*
 * Move the motor with sub-degree precision.
 * Note that calling this function will increase program size substantially
 * due to inclusion of float support.
 */
void LEDCStepperDriver::startRotate(double deg)
{
    startMove(calcStepsForRotation(deg));
}
/*
 * Enable/Disable the motor by setting a digital flag
 */
void LEDCStepperDriver::enable(void)
{
    if IS_CONNECTED (enable_pin)
    {
        digitalWrite(enable_pin, enable_active_state);
    };
    usleep(2);
}
void LEDCStepperDriver::disable(void)
{
    if IS_CONNECTED (enable_pin)
    {
        digitalWrite(enable_pin, (enable_active_state == HIGH) ? LOW : HIGH);
    }
}
/*
 * Immediate stop
 * Returns the number of steps remaining.
 */
long LEDCStepperDriver::stop()
{
    driver_pwm_stop();

    int16_t counter_value;
    pcnt_get_counter_value(pcnt_unit, &counter_value);
    steps_remaining -= counter_value;
    pcnt_counter_pause(pcnt_unit);
    pcnt_counter_clear(pcnt_unit);

    if (auto_sleep)
        disable();

    return steps_remaining;
}