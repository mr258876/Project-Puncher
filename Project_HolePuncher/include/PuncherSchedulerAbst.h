#ifndef _PUNCHERSCHEDULERABST_H_
#define _PUNCHERSCHEDULERABST_H_

#include "PuncherSchedulerTyping.h"

class PuncherSchedulerInterface
{

public:
    virtual int start_workload() = 0;
    virtual int pause_workload() = 0;
    virtual int delete_workload() = 0;
    virtual int add_hole(scheduler_hole_t h) = 0;
    virtual int feed_paper(int gear) = 0;
    virtual unsigned int set_status(unsigned int status_code) = 0;
    virtual unsigned int get_status() = 0;
    virtual time_t get_ETA() = 0;
};

#endif