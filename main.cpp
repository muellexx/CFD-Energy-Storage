#include <iostream>
#include <omp.h>

#include "file.h"
#include "init.h"
#include "struct.h"
#include "state.h"
#include "temp.h"
#include "timer.h"
#include "mms.h"
#include "energy.h"

# define PI           3.14159265358979323846  /* pi */


using namespace std;



int main(int argc, char **argv)
{

    timer.t0 = TIMER_getWTime();
    INIT_initializeParameters(argc,argv);

    if (mms.flagOfMms){
        INIT_initializeMms();
    }

    energy.thermEnergy0 = ENERGY_capacity();

    while (timer.iTimeStep<timer.finishTimeStep){

        if (mms.flagOfMms)
            MMS_Calculate();
        else
            TEMP_FTBS();

        ENERGY_exergy();

        //if (timer.iTimeStep%100000==0){
        if (parameter.flagOfExactSol && timer.time >= 5000 && timer.time < 5000 + timer.dt){
            FILE_writeTemperature();
            if (parameter.flagOfExactSol){
                cout << "execution time: " << TIMER_getWTime() - timer.t0 << " s" << endl;
                return 0;
            }
        }

        //FILE_stateOutput();

        if (mms.flagOfMms){
            FILE_mmsCheckFinalize();
            if (mms.finishf + mms.finishs == 2)
                return 0;
        }

        timer.iTimeStep+=1;
        timer.time+=timer.dt;
        timer.timeInCycle+=timer.dt;
        ENERGY_check();
        STATE_checkState();
    }
    cout << "execution time: " << TIMER_getWTime() - timer.t0 << " s" << endl;

    return 0;
}
