#ifndef _PUNCHER_SCHEDULER_H_
#define _PUNCHER_SCHEDULER_H_

#include "lib/LinkedList/LinkedList.h"

struct Hole
{
    int x;
    float z;
};

extern int punchETA;
extern unsigned long punchStartTime;
extern int puncherStatus;
extern LinkedList<Hole> holeList;
extern int holeCount;
extern int holeFinished;

int calcETA(int Xrpm, int Yrpm, int Zrpm);

#endif
