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
    ListNode<Hole>* current = holeList.getRoot();

    while (current->next != NULL)
    {
        _eta += abs(current->data.x - _lastX) * 2.0 / 8 / Xspeed;
        _eta += 8 / Yspeed;
        current = current->next;
    }
    _eta += current->data.z / Zspeed ;
    _eta += 8 / Yspeed;

    punchETA = (int)_eta;
    return (int)_eta;
}
