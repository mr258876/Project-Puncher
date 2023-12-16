#include "PuncherScheduler.h"

PuncherScheduler::PuncherScheduler()
{
    this->holeListHandle = xSemaphoreCreateMutex();
}

PuncherScheduler::~PuncherScheduler()
{
}

void begin()
{
}

void PuncherScheduler::onUINotify(void *param)
{

}

int PuncherScheduler::start_workload()
{
    if (puncher_is_busy(status))
        return status.basic_status.status_data;

    if (status.basic_status.status_flags.has_mission)
    {
        reverse(holeList.begin(), holeList.end()); // Reverse the vector so it will be O(1) each time poping a hole

        // TODO


        status.basic_status.status_flags.is_running = 1;
        return 0;
    }
    return 1;
}

int PuncherScheduler::pause_workload()
{
    // TODO

    return 0;
}

int PuncherScheduler::delete_workload()
{
    if (puncher_is_busy(this->status))
    {
        return this->status.basic_status.status_data;
    }

    this->holeList.clear();
    return 0;
}

int PuncherScheduler::add_hole(scheduler_hole_t h)
{
    holeList.push_back(h);
    return 0;
}

int PuncherScheduler::feed_paper(int gear)
{
    if (puncher_is_busy(this->status))
        return this->status.basic_status.status_data;

    // TODO
    if (gear)
    {
        // this->Z.sleep(false);
        this->Z->rotate_infinite(gear);
        this->status.basic_status.status_flags.is_feeding_paper = 1;
    }
    else
    {
        this->Z->stop();
        // this->Z.sleep(true);
        this->status.basic_status.status_flags.is_feeding_paper = 0;
    }

    return 0;
}

int PuncherScheduler::ui_get_menu()
{
    // TODO

    return 0;
}

unsigned int PuncherScheduler::set_status(unsigned int status_code)
{
    // TODO

    return status.basic_status.status_data;
}

unsigned int PuncherScheduler::get_status()
{
    return status.basic_status.status_data;
}

time_t PuncherScheduler::get_ETA()
{
    double _eta = 0;

    int _lastX = 0;
    double _lastZ = 0;

    for (size_t i = 0; i < holeList.size(); i++)
    {
        _eta += abs(holeList.at(i).x - _lastX) * 2.0 / X->getSpeed(); // X轴
        _eta += abs(holeList.at(i).z - _lastZ) / Z->getSpeed();       // Y轴
        _eta += 11 / Y->getSpeed();                                   // Z轴

        _lastX = holeList.at(i).x;
        _lastZ = holeList.at(i).z;
    }

    return (long)_eta;
}