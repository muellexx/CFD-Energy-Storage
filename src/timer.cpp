#include <iostream>
#include <sys/time.h>
#include <omp.h>
#include <math.h>

#include "struct.h"
#include "timer.h"


using namespace std;

void
TIMER_initializeTimer(int argc, char **argv){

    timer.time = 0;
    timer.timeInCycle = 0;
    timer.t1 = TIMER_getWTime();
    timer.t2 = TIMER_getWTime();

    timer.cycleTime = (state.chargingDuration + state.firstIdleDuration + state.dischargingDuration + state.secondIdleDuration)*3600;
    timer.finishTime = state.numberOfCycles*timer.cycleTime;

    timer.iTimeStep=0;

    if (mms.flagOfMms)
        timer.finishTimeStep = 1000000;
    if (!mms.flagOfMms){
        timer.finishTimeStep = timer.finishTime/timer.dt;
        //timer.finishTimeStep = state.numberOfCycles*state.timeStepsPerCycle;
        //timer.dt = timer.finishTime/timer.finishTimeStep;
    }

}

double
TIMER_getWTime(void)
{
#if defined(_OPENMP)
	return omp_get_wtime();
#else
	struct timeval t;

	gettimeofday(&t, NULL);
	return (double)t.tv_sec + (double)t.tv_usec*1.0e-6;
#endif
}

void
TIMER_convert(double t){
    timer.s = t;
    timer.m = timer.s / 60;
    timer.h = timer.m / 60;
    timer.m = timer.m%60;
    timer.s = fmod(timer.s,60);
}
