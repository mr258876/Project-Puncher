#ifndef _PUNCHER_SCHEDULER_H_
#define _PUNCHER_SCHEDULER_H_

#include "lib/LinkedList/LinkedList.h"

struct Hole
{
    int x;
    double z;
};

extern int punchETA;
extern unsigned long punchStartTime;
extern int puncherStatus;
extern LinkedList<Hole> holeList;
extern int holeCount;
extern int holeFinished;

int calcETA(double Xspeed, double Yspeed, double Zspeed);

#endif
