/***********************************************************************
* Component:
*    Scheduler RR
* Author:
*    Ted McDaniel
* Summary: 
*    This is the base-class that enables various schedule algorithms
*    to simulate CPU Scheduling
************************************************************************/

#ifndef SCHEDULER_RR
#define SCHEDULER_RR

#include <queue>      // For the ready queue
#include "schedule.h"

using namespace std;

/****************************************************
 * RR
 * The Round Robin scheduler
 ***************************************************/
class SchedulerRR : public Disbatcher
{
public:
   SchedulerRR(int q) : Disbatcher(),
      timeQuantaDuration(q) {}

   // a new process has just been executed
   void startProcess(int pid)
   {
	// In Round Robin, ready queue is FIFO
	readyQueue.push(pid);
   }
      
   // execute one clock cycle
   bool clock()
   {
	if (time % timeQuantaDuration == 0)
	{
		// Is current process finished or is no process selected?
		if (pidCurrent == PID_NONE || processes[pidCurrent].isDone())
		{
			// If there's something in a ready queue
			if (readyQueue.size())
			{
				pidCurrent = readyQueue.front();
				readyQueue.pop();
			}
			else // nothing is in the ready queue
				pidCurrent = PID_NONE;
		}	
	}
	time++;
      return Disbatcher::clock();
   }

  private:
   int timeQuantaDuration;      // How long is the time interval?
   std::queue <int> readyQueue; // FIFO queue to hold processes
};

#endif // SCHEDULE_RR
