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

int calcETA(int Xrpm, int Yrpm, int Zrpm)
{   
    float _eta = 0;

    int _lastX = 0;
    ListNode<Hole>* current = holeList.getRoot();

    while (current->next != NULL)
    {
        _eta += current->data.x * 25.852579588531222053828834659495 / Zrpm / 1600;
        _eta += abs(current->data.x - _lastX) * 2.0 / 8 / Xrpm;
        _eta += 8 / 8 /Yrpm;
        current = current->next;
    }
    _eta = _eta * 60; // 之前是以分钟为单位
    punchETA = (int)_eta;
    return (int)_eta;
}
