#include <iostream>
#include <math.h>
#include <omp.h>

#include "struct.h"
#include "file.h"
#include "energy.h"

# define PI           3.14159265358979323846  /* pi */

using namespace std;

void
ENERGY_check(){
    if (timer.timeInCycle>=state.cycleDuration*3600 && timer.timeInCycle < state.cycleDuration*3600 + timer.dt){
        energy.efficiency = (energy.exergyDout - energy.exergyDin)/(energy.exergyCin - energy.exergyCout);
        FILE_writeResults();
        energy.exergyCin = 0;
        energy.exergyCout = 0;
        energy.exergyDin = 0;
        energy.exergyDout = 0;
        energy.thermEnergy0 = ENERGY_capacity();
        FILE_writeTemperature();
    }
}

void
ENERGY_exergy(){
    double tempRef = 288.15;
    if (state.state==0){
        energy.exergyCin += timer.dt*fluid.massFlow*fluid.specHeatCap*(fluid.inflowTemp - tempRef*(1 + log(fluid.inflowTemp/tempRef)));
        energy.exergyCout += timer.dt*fluid.massFlow*fluid.specHeatCap*(fluid.temperatureNew[storage.numberOfCells-1] -tempRef*(1+log(fluid.temperatureNew[storage.numberOfCells-1]/tempRef)));
    }
    if (state.state==1){
        energy.exergyDin += timer.dt*fluid.massFlow*fluid.specHeatCap*(fluid.initTemp - tempRef*(1 + log(fluid.initTemp/tempRef)));
        energy.exergyDout += timer.dt*fluid.massFlow*fluid.specHeatCap*(fluid.temperatureNew[0] -tempRef*(1+log(fluid.temperatureNew[0]/tempRef)));
    }

}

void
ENERGY_efficiency(){
    energy.efficiency = (energy.exergyDout - energy.exergyDin)/(energy.exergyCin - energy.exergyCout);
}

double
ENERGY_capacity(){
    double thermEnergy;
    double f = 0.;
    double s = 0.;

    for (int i=0; i<storage.numberOfCells; i++){
        f += fluid.temperatureNew[i] - fluid.initTemp;
        s += solid.temperatureNew[i] - fluid.initTemp;
    }
    f = f*parameter.epsilon*fluid.density*fluid.specHeatCap*storage.cellLength;
    s = s*(1-parameter.epsilon)*solid.density*solid.specHeatCap*storage.cellLength;
    thermEnergy = (f+s)*PI*storage.diameter*storage.diameter/4.;
    energy.maxThemEnergy = (parameter.epsilon*fluid.density*fluid.specHeatCap + (1-parameter.epsilon)*solid.density*solid.specHeatCap)*PI*storage.diameter*storage.diameter*storage.height*(fluid.inflowTemp-fluid.initTemp)/4.;
    return thermEnergy;
}
