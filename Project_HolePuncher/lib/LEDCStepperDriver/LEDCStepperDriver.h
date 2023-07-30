
#ifndef _LEDCSTEPPERDRIVER_H_
#define _LEDCSTEPPERDRIVER_H_

#include <Arduino.h>
#include "driver/ledc.h"
#include "soc/ledc_struct.h"
#include "driver/pcnt.h"
#include "soc/pcnt_struct.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "soc/rtc_wdt.h"

#define PIN_UNCONNECTED -1
#define IS_CONNECTED(pin) (pin != PIN_UNCONNECTED)

#define PCNT_L_LIM_VAL -1
#define PCNT_H_LIM_VAL 32760
#define PCNT_THRES_VAL 16000

class LEDCStepperDriver
{
private:
    /*
     * ESP32 Peripherals
     */
    ledc_mode_t ledc_mode;
    ledc_timer_t ledc_timer;
    ledc_channel_t ledc_channel;
    pcnt_unit_t pcnt_unit;
    pcnt_channel_t pcnt_channel;
    pcnt_isr_handle_t pcnt_isr_handle;
    bool count_falling_edge;

    void driver_pwm_start();
    void driver_pwm_stop();
    void driver_pcnt_start(long steps);
    friend void driver_pcnt_intr_handler(void *arg);
    // friend void driver_pcnt_task(void *arg);

    /*
     * Motor Configuration
     */
    short motor_steps; // motor steps per revolution (usually 200)

    /*
     * Driver Configuration
     */
    short dir_pin;
    short step_pin;
    short enable_pin = PIN_UNCONNECTED;
    short enable_active_state = HIGH;
    short microsteps = 1;
    float rpm = 0;
    bool auto_sleep = false;

    /*
     * Movement state
     */
    long step_count;      // step count
    long steps_remaining; // to complete the current move (absolute value)
    long pulse_freq;      // step pulse frequency (hz)
    uint8_t infinite_run;

    // microstep range (1, 16, 32 etc)
    static const short MAX_MICROSTEP = 128;

    // DIR pin state
    short dir_state = HIGH;

    bool pwm_running;
    bool pcnt_running;

public:
    LEDCStepperDriver(int motor_steps, int dir_pin, int step_pin, int enable_pin, int driver_id);
    LEDCStepperDriver(int motor_steps, int dir_pin, int step_pin, int enable_pin, ledc_mode_t ledc_mode, ledc_timer_t ledc_timer, ledc_channel_t ledc_channel, pcnt_unit_t pcnt_unit, pcnt_channel_t pcnt_channel = PCNT_CHANNEL_0, bool count_falling_edge = false);
    ~LEDCStepperDriver();

    void begin(float rpm = 60, short micro_steps = 1);
    /*
     * Set current microstep level, 1=full speed, 32=fine microstepping
     * Returns new level or previous level if value out of range
     */
    virtual short setMicrostep(short microsteps);
    short getMicrostep(void)
    {
        return microsteps;
    }
    short getSteps(void)
    {
        return motor_steps;
    }
    /*
     * Set target motor RPM (1-200 is a reasonable range)
     */
    void setRPM(float rpm);
    void setPulseFreq(long freq);
    float getRPM(void)
    {
        return rpm;
    }
    float getCurrentRPM(void)
    {
        return (60.0 * pulse_freq / microsteps / motor_steps);
    }
    /*
     * Move the motor a given number of steps.
     * positive to move forward, negative to reverse
     */
    void move(long steps);
    /*
     * Rotate the motor a given number of degrees (1-360)
     */
    void rotate(long deg);
    inline void rotate(int deg)
    {
        rotate((long)deg);
    };
    /*
     * Rotate using a float or double for increased movement precision.
     */
    void rotate(double deg);
    /*
     * Rotate infinitely. Use stop() or move(0) to stop.
     */
    void rotate_infinite(int dir);
    /*
     * For compatibility.
     */
    void startMove(long steps);
    inline void startRotate(int deg)
    {
        startRotate((long)deg);
    };
    void startRotate(long deg);
    void startRotate(double deg);
    /*
     * Configure which logic state on ENABLE pin means active
     * when using SLEEP (default) this is active HIGH
     */
    void setEnableActiveState(short state)
    {
        enable_active_state = state;
    }
    /*
     * Turn off/on motor to allow the motor to be moved by hand/hold the position in place
     */
    void enable(void);
    void disable(void);
    /*
     * Whether to turn off motor automatically
     */
    void setAutoSleep(bool auto_sleep_enable)
    {
        auto_sleep = auto_sleep_enable;
    }
    bool getAutoSleep()
    {
        return auto_sleep;
    }
    /*
     * Immediate stop
     * Returns the number of steps remaining.
     */
    long stop();
    /*
     * Get the number of completed steps so far.
     * This is always a positive number
     */
    long getStepsCompleted(void)
    {
        return step_count;
    }
    /*
     * Get the number of steps remaining to complete the move
     * This is always a positive number
     */
    long getStepsRemaining(void)
    {
        return steps_remaining;
    }
    /*
     * Get movement direction: forward +1, back -1
     */
    int getDirection(void)
    {
        return (dir_state == HIGH) ? 1 : -1;
    }
    /*
     * Return calculated time to complete the given move
     */
    long getTimeForMove(long steps);
    /*
     * Calculate steps needed to rotate requested angle, given in degrees
     */
    long calcStepsForRotation(long deg)
    {
        return deg * motor_steps * (long)microsteps / 360;
    }
    long calcStepsForRotation(double deg)
    {
        return deg * motor_steps * microsteps / 360;
    }
    bool isRunning()
    {
        return steps_remaining != 0;
    }
};

#endif