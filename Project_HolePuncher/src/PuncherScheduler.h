#ifndef _PUNCHER_SCHEDULER_H_
#define _PUNCHER_SCHEDULER_H_

#include "LinkedList.h"

struct Hole
{
    int x;
    double z;
};

extern int punchETA;    // 预计剩余时间(秒)
extern unsigned long punchStartTime;    // 打孔开始时间(相对于开机时间, 毫秒)
extern int puncherStatus;   // 打孔机状态(见状态表)
extern LinkedList<Hole> holeList;   // 孔位储存链表
extern int holeCount;   // 当前任务总孔数
extern int holeFinished;    // 当前任务已完成孔数
extern double lengthZ; // z轴位置(mm)

int calcETA(double Xspeed, double Yspeed, double Zspeed);

#endif
