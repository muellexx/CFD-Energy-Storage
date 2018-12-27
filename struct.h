#ifndef STRUCT_H_INCLUDED
#define STRUCT_H_INCLUDED

#include <vector>

using namespace std;

struct structStorage{

    double height;
    double diameter;
    double volume;
    int numberOfCells;
    double cellLength;

};

struct structCell{
    vector <double> position;
};

struct structFluid{

    double initTemp;
    double inflowTemp;
    vector <double> temperatureOld;
    vector <double> temperatureNew;
    double diffusivity;
    double initVelocity;
    double massFlow;
    double velocity;
    double density;
    double specHeatCap;
    double conductivity;
    double mu;
    double heatTransfer;
    double tempIncrease;
};

struct structSolid{

    double initTemp;
    vector <double> temperatureOld;
    vector <double> temperatureNew;
    double diffusivity;
    double density;
    double specHeatCap;
    double conductivity;
    double diameter;
    double heatTransfer;
};

struct structParameter{

    char nameOfInputFile[100];
    char nameOfOutputTemperature[100];
    char nameOfDividedOutputTemperature[100];
    char nameOfOutputState[100];
    char nameOfLogFile[100];
    char nameOfOutputMms[100];
    char nameOfDividedOutputMms[100];
    char nameOfFinalMms [100];
    char nameOfResults[100];
    double epsilon;
    double cfl;
    double d;
    double prandtl;
    double reynold;
    double nusselt;
    double fsHeatTransfer;
    double heatTransferCoeff;
    double volHeatTransferCoeff;
    int threads;
    int flagOfExactSol;
};

struct structTimer{
    double dt;
    int iTimeStep;
    double time;
    double timeInCycle;

    double finishTime;
    int finishTimeStep;

    double t0;
    double t1;
    double t2;

    double s;
    int m;
    int h;

    double cycleTime;           //time needed for one cycle
};

struct structState{
    int state;
    int numberOfCycles;
    int timeStepsPerCycle;
    int iCycle;

    double chargingDuration;
    double firstIdleDuration;
    double dischargingDuration;
    double secondIdleDuration;
    double cycleDuration;

};

struct structMms{
    vector <double> sManf;
    vector <double> sMans;
    double errorf;
    double errorfEnd;
    double errors;
    double errorsEnd;
    double maxerror;
    double errorfOld;
    double errorsOld;
    double criterium;
    int n;
    double k;
    int flagOfMms;
    double peclet;
    int finishf;
    int finishs;

};

struct structEnergy{
    double exergyCin;
    double exergyCout;
    double exergyDin;
    double exergyDout;
    double efficiency;
    double maxThemEnergy;
    double thermEnergy0;
    double thermEnergy1;
    double capacityFactor;

};


extern struct structStorage     storage;
extern struct structCell        cell;
extern struct structFluid       fluid;
extern struct structSolid       solid;
extern struct structParameter   parameter;
extern struct structTimer       timer;
extern struct structState       state;
extern struct structMms         mms;
extern struct structEnergy      energy;

#endif // STRUCT_H_INCLUDED
