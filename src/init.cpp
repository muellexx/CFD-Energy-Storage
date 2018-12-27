#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>

#include "struct.h"
#include "file.h"
#include "timer.h"
#include "init.h"
#include "state.h"
#include "temp.h"

# define PI           3.14159265358979323846  /* pi */

using namespace std;

void
INIT_initializeArguments(int argc,char **argv){
    storage.diameter = 0;
    for(int i = 1; i < argc; i++ ) {
        if (strcmp( argv[i], "-in" ) == 0 ) {
            strcpy(parameter.nameOfInputFile,argv[i+1]);
            i++;
        }
        if (strcmp(argv[i], "-mms") == 0){
            mms.flagOfMms = atoi(argv[i+1]);
            i++;
        }
        if (strcmp(argv[i], "-d") == 0){
            storage.diameter = atof(argv[i+1]);
        }
        if (strcmp(argv[i], "-t") == 0){
            parameter.threads = atoi(argv[i+1]);
        }
    }
}

void
INIT_initializeMmsArguments(int argc, char **argv){
    for(int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-dt") == 0) {
            timer.dt = atof(argv[i+1]);
            i++;
        }
        if (strcmp(argv[i], "-n") == 0) {
            storage.numberOfCells = atoi(argv[i+1]);
            i++;
        }
        if (strcmp(argv[i], "-uf") == 0) {
            fluid.initVelocity = atof(argv[i+1]);
            i++;
        }
        if (strcmp(argv[i], "-af") == 0) {
            fluid.diffusivity = atof(argv[i+1]);
            solid.diffusivity = fluid.diffusivity;
            i++;
        }
    }
}


void
INIT_initializeParameters(int argc,char **argv){

    strcpy(parameter.nameOfInputFile, "input.data");
    strcpy(parameter.nameOfResults, "Mueller.dat");

    mms.flagOfMms = 5;
    parameter.threads = 4;

    INIT_initializeArguments(argc, argv);

    cout << "using " << parameter.threads << " threads for openMP" << endl;

    FILE_readDataFile();

    if (!storage.height)
        storage.height = 4.*storage.volume/(PI*storage.diameter*storage.diameter);
    else
        storage.volume = storage.height*PI*storage.diameter*storage.diameter/4.;

    if (mms.flagOfMms)
        INIT_initializeMmsArguments(argc,argv);

    if (!mms.flagOfMms)
        INIT_initializeTemperature();

    storage.cellLength = storage.height/(double)storage.numberOfCells;

    for (int i=0; i<storage.numberOfCells; i++)
        cell.position.push_back(storage.cellLength*(i+0.5));


    if (!mms.flagOfMms)
        INIT_calculateVelocity();

    fluid.velocity = fluid.initVelocity;
    energy.exergyCin = 0;
    energy.exergyCout = 0;
    energy.exergyDin = 0;
    energy.exergyDout = 0;

    FILE_initializeLogFile();

    TIMER_initializeTimer(argc, argv);
    FILE_initializeLogFile();

    INIT_checkCFLNumber();

    INIT_initializeHeatTransfer();

    FILE_initializeLogFile();

    state.state = 0;  //0 = Charging, 1 = Discharging, 2 = Idle
    state.cycleDuration = state.chargingDuration+state.dischargingDuration+state.firstIdleDuration+state.secondIdleDuration;
    state.iCycle = 1;

    FILE_initializeStateOutput();

    FILE_initializeResults();

}

void
INIT_initializeTemperature(void){
    for (int i=0; i<storage.numberOfCells; i++){
        fluid.temperatureOld.push_back(fluid.initTemp);
        fluid.temperatureNew.push_back(fluid.initTemp);
        solid.temperatureOld.push_back(solid.initTemp);
        solid.temperatureNew.push_back(solid.initTemp);
    }
}

void
INIT_calculateVelocity(void){
    fluid.initVelocity = 4*fluid.massFlow/(parameter.epsilon*fluid.density*PI*storage.diameter*storage.diameter);
    STATE_setState();
}

void
INIT_checkCFLNumber(void){
    parameter.cfl = fluid.velocity*timer.dt/storage.cellLength;
    parameter.d = fluid.diffusivity*timer.dt/(storage.cellLength*storage.cellLength);

}

void
INIT_initializeHeatTransfer(void){
    mms.peclet = fluid.velocity*storage.height/fluid.diffusivity;
    if (parameter.flagOfExactSol){
        fluid.diffusivity = 0; //fluid.conductivity/(parameter.epsilon*fluid.density*fluid.specHeatCap);
        solid.diffusivity = 0; //solid.conductivity/((1-parameter.epsilon)*solid.density*solid.specHeatCap);
    }
    else if (!mms.flagOfMms){
        fluid.diffusivity = fluid.conductivity/(parameter.epsilon*fluid.density*fluid.specHeatCap);
        solid.diffusivity = solid.conductivity/((1-parameter.epsilon)*solid.density*solid.specHeatCap);
    }
    if (mms.flagOfMms==2){
        fluid.conductivity = fluid.diffusivity*parameter.epsilon*fluid.density*fluid.specHeatCap;
        solid.conductivity = solid.diffusivity*(1-parameter.epsilon)*solid.density*solid.specHeatCap;
    }
    parameter.prandtl = fluid.mu*fluid.specHeatCap/fluid.conductivity;
    parameter.reynold = parameter.epsilon*fluid.initVelocity*fluid.density*solid.diameter/fluid.mu;
    parameter.nusselt = 0.255*pow(parameter.prandtl,1.0/3.0)*pow(parameter.reynold,2.0/3.0)/parameter.epsilon;
    parameter.fsHeatTransfer = parameter.nusselt*fluid.conductivity/solid.diameter;
    parameter.heatTransferCoeff = 1/(1/parameter.fsHeatTransfer + solid.diameter/(10*solid.conductivity));
    parameter.volHeatTransferCoeff = 6*(1-parameter.epsilon)*parameter.heatTransferCoeff/solid.diameter;

    fluid.heatTransfer = parameter.volHeatTransferCoeff/(parameter.epsilon*fluid.density*fluid.specHeatCap);
    solid.heatTransfer = parameter.volHeatTransferCoeff/((1-parameter.epsilon)*solid.density*solid.specHeatCap);

    energy.maxThemEnergy = (parameter.epsilon*fluid.density*fluid.specHeatCap + (1-parameter.epsilon)*solid.density*solid.specHeatCap)*PI*storage.diameter*storage.diameter*storage.height*(fluid.inflowTemp-fluid.initTemp)/4.;

}

void
INIT_initializeMms(void){

    mms.k = 2*PI*(double)mms.n/storage.height;
    mms.errorf=0.;
    mms.errors=0.;
    strcpy(parameter.nameOfFinalMms, "mmsfinal.out");
    mms.finishf = 0;
    mms.finishs = 0;

    FILE_initializeMmsOutput();

    if (mms.flagOfMms == 1){
        for (int i=0; i<storage.numberOfCells; i++){
            mms.sManf.push_back((-1.0)*fluid.velocity*mms.k*sin(mms.k*cell.position[i]) + fluid.diffusivity*mms.k*mms.k*cos(mms.k*cell.position[i]));
            mms.sMans.push_back(solid.diffusivity*mms.k*mms.k*cos(mms.k*cell.position[i]));
        }
    }
    else if (mms.flagOfMms == 2){
        for (int i=0; i<storage.numberOfCells; i++){
            mms.sManf.push_back((-1.0)*fluid.velocity*mms.k*sin(mms.k*cell.position[i]) + fluid.diffusivity*mms.k*mms.k*cos(mms.k*cell.position[i]));
            mms.sMans.push_back(solid.diffusivity*mms.k*mms.k*cos(mms.k*cell.position[i]));
        }
    }
    for (int i=0; i<storage.numberOfCells; i++){
        fluid.temperatureOld.push_back(cos(mms.k*cell.position[i]));
        fluid.temperatureNew.push_back(cos(mms.k*cell.position[i]));
        solid.temperatureOld.push_back(cos(mms.k*cell.position[i]));
        solid.temperatureNew.push_back(cos(mms.k*cell.position[i]));
    }
}
