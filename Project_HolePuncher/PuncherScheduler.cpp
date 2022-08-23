#include "PuncherScheduler.h"

extern int punchETA = 0;
extern unsigned long punchStartTime = 0;
extern int puncherStatus = 0x10;
extern LinkedList<Hole> holeList = LinkedList<Hole>();
extern int holeCount = 0;
extern int holeFinished = 0;

int abs(int a)
{
    return a>0? a:-a;
}

int calcETA(double Xspeed, double Yspeed, double Zspeed)
{   
    double _eta = 0;

    int _lastX = 0;
    double _lastZ = 0;
    ListNode<Hole>* current = holeList.getRoot();

    while (current->next != NULL)
    {
        _eta += abs(current->data.x - _lastX) * 2.0 / Xspeed;   // X轴
        _eta += abs(current->data.z - _lastZ) / Zspeed;         // Y轴
        _eta += 11 / Yspeed;                                    // Z轴

        _lastX = current->data.x;
        _lastZ = current->data.z;

        current = current->next;
    }
    _eta += abs(current->data.x - _lastX) * 2.0 / Xspeed;
    _eta += abs(current->data.z - _lastZ) / Zspeed;
    _eta += 11 / Yspeed;

    punchETA = (int)_eta;
    return (int)_eta;
}
