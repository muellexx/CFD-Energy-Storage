#include <iostream>
#include <fstream>

#include "struct.h"
#include "state.h"
#include "timer.h"
#include "energy.h"
#include "file.h"

using namespace std;

void
STATE_checkState(void){
    if (timer.timeInCycle>=timer.cycleTime){
        ofstream outputfile;
        outputfile.open (parameter.nameOfLogFile, ios::app);

        timer.t2 = TIMER_getWTime();
        timer.timeInCycle = timer.timeInCycle - timer.cycleTime;
        TIMER_convert(timer.t2-timer.t1);
        cout << "Cylce number " << state.iCycle << " has finished in: ";
        outputfile << "Cylce number " << state.iCycle << " has finished in: ";
        if (timer.h != 0){
            cout << timer.h << " h, ";
            outputfile << timer.h << " h, ";
        }
        if(timer.h!=0 ||timer.m !=0){
            cout << timer.m << " min and ";
            outputfile << timer.m << " min and ";
        }
        cout << timer.s << " s" << endl;
        outputfile << timer.s << " s" << endl;
        TIMER_convert(timer.t2-timer.t0);
        cout << "and the current run time is:    ";
        outputfile << "and the current run time is:    ";
        if (timer.h != 0){
            cout << timer.h << " h, ";
            outputfile << timer.h << " h, ";
        }
        if(timer.h!=0 ||timer.m !=0){
            cout << timer.m << " min and ";
            outputfile << timer.m << " min and ";
        }
        cout << timer.s << " s" << endl;
        outputfile << timer.s << " s" << endl;
        timer.t1 = TIMER_getWTime();
        state.iCycle+=1;
        outputfile.close();
    }
    STATE_setState();
}

void
STATE_setState(void){
    if (timer.timeInCycle<=state.chargingDuration*3600){
        state.state = 0;
        fluid.velocity = fluid.initVelocity;
    }
    else if (timer.timeInCycle<=(state.chargingDuration+state.firstIdleDuration)*3600){
        if (timer.timeInCycle>state.chargingDuration*3600 && timer.timeInCycle <= state.chargingDuration*3600 + timer.dt){
            fluid.tempIncrease = fluid.temperatureNew[storage.numberOfCells-1] - fluid.initTemp;
        }
        state.state = 2;
        fluid.velocity = 0;
    }
    else if (timer.timeInCycle<=(state.chargingDuration+state.firstIdleDuration+state.dischargingDuration)*3600){
        state.state = 1;
        fluid.velocity = fluid.initVelocity;
        if (timer.timeInCycle>(state.chargingDuration+state.firstIdleDuration)*3600 && timer.timeInCycle <= (state.chargingDuration+state.firstIdleDuration)*3600 + timer.dt){
            energy.thermEnergy1 = ENERGY_capacity();
            energy.capacityFactor = (energy.thermEnergy1-energy.thermEnergy0)/energy.maxThemEnergy;
            FILE_writeTemperature();
        }
    }
    else{
        state.state = 2;
        fluid.velocity = 0;
    }
}
