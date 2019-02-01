/***********************************************************************
* Component:
*    Scheduler SRT
* Author:
*    your name
* Summary: 
*    This is the base-class that enables various schedule algorithms
*    to simulate CPU Scheduling
************************************************************************/

#ifndef SCHEDULER_SRT
#define SCHEDULER_SRT

#include "schedule.h"


/****************************************************
 * SRT
 * The Shortest Remaining Time Scheduler
 ***************************************************/
class SchedulerSRT : public Disbatcher
{
public:
   SchedulerSRT() : Disbatcher() {}

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

#endif // SCHEDULE_SRT
