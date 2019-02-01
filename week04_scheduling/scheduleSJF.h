/***********************************************************************
* Component:
*    Scheduler SJF
* Author:
*    Ted McDaniel
* Summary: 
*    This is the base-class that enables various schedule algorithms
*    to simulate CPU Scheduling
************************************************************************/

#ifndef SCHEDULER_SJF
#define SCHEDULER_SJF

#include "schedule.h"


/****************************************************
 * SJF
 * The Shortest Job First scheduler
 ***************************************************/
class SchedulerSJF : public Disbatcher
{
public:
   SchedulerSJF() : Disbatcher() {}

   // a new process has just been executed
   void startProcess(int pid)
   {
   }
   
   
   // execute one clock cycle
   bool clock()
   {
      return Disbatcher::clock();
   }

private:
};

#endif // SCHEDULE_SJF
