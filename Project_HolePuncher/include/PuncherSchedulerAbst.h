#ifndef _PUNCHERSCHEDULERABST_H_
#define _PUNCHERSCHEDULERABST_H_

#include "PuncherSchedulerTyping.h"
#include "PuncherEvents.h"
#include <time.h>

class PuncherSchedulerInterface
{

public:
    virtual int start_workload() = 0;
    virtual int pause_workload() = 0;
    virtual int delete_workload() = 0;
    virtual int data_transmit_mode(bool transmit_mode) = 0;
    virtual int feed_paper_mode(bool feed_paper_mode) = 0;
    virtual int add_hole(scheduler_hole_t &h) = 0;
    virtual int add_hold_mcode(int x, int y, int z) = 0;
    virtual int feed_paper(int gear) = 0;
    virtual unsigned int set_status(unsigned int status_code) = 0;
    virtual unsigned int get_status() = 0;
    virtual time_t get_ETA() = 0;
    virtual void set_setting_value(puncher_event_setting_change_t *evt) = 0;
    virtual void get_setting_values(void *ui) = 0;
    virtual void get_status(void *ui) = 0;
};

#endif