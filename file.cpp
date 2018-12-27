#include <iostream>
#include <fstream>
#include <math.h>
#include <ctime>


#include "struct.h"
#include "file.h"
#include "timer.h"

using namespace std;


void
FILE_readDataFile(void){
    ifstream inputfile;
    ifstream ifile(parameter.nameOfInputFile);
    if (!ifile) {
        cout << "Input File with the name " << parameter.nameOfInputFile << " does not exist." << endl;
        exit(1);
    }

    inputfile.open (parameter.nameOfInputFile);
    char tmp[100];

    if (inputfile.is_open())
  {
      inputfile >> tmp;
      inputfile >> tmp;
      inputfile >> tmp;
      inputfile >> parameter.nameOfLogFile;
      inputfile >> tmp;
      inputfile >> tmp;
      inputfile >> storage.height;
      inputfile >> tmp;
      if (!storage.diameter)
          inputfile >> storage.diameter;
      else
          inputfile >> tmp;
      inputfile >> tmp;
      if (!storage.height)
          inputfile >> storage.volume;
      else
          inputfile >> tmp;
      inputfile >> tmp;
      inputfile >> storage.numberOfCells;
      inputfile >> tmp;
      inputfile >> tmp;
      inputfile >> fluid.initTemp;
      inputfile >> tmp;
      inputfile >> fluid.inflowTemp;
      inputfile >> tmp;
      inputfile >> fluid.diffusivity;
      inputfile >> tmp;
      inputfile >> fluid.initVelocity;
      inputfile >> tmp;
      inputfile >> fluid.massFlow;
      inputfile >> tmp;
      inputfile >> fluid.density;
      inputfile >> tmp;
      inputfile >> fluid.specHeatCap;
      inputfile >> tmp;
      inputfile >> fluid.conductivity;
      inputfile >> tmp;
      inputfile >> fluid.mu;
      inputfile >> tmp;
      inputfile >> tmp;
      inputfile >> solid.initTemp;
      inputfile >> tmp;
      inputfile >> solid.diffusivity;
      inputfile >> tmp;
      inputfile >> solid.density;
      inputfile >> tmp;
      inputfile >> solid.diameter;
      inputfile >> tmp;
      inputfile >> solid.specHeatCap;
      inputfile >> tmp;
      inputfile >> solid.conductivity;
      inputfile >> tmp;
      inputfile >> tmp;
      inputfile >> parameter.nameOfOutputTemperature;
      inputfile >> tmp;
      inputfile >> tmp;
      inputfile >> timer.finishTime;
      inputfile >> tmp;
      inputfile >> timer.dt;
      inputfile >> tmp;
      inputfile >> tmp;
      inputfile >> state.chargingDuration;
      inputfile >> tmp;
      inputfile >> state.firstIdleDuration;
      inputfile >> tmp;
      inputfile >> state.dischargingDuration;
      inputfile >> tmp;
      inputfile >> state.secondIdleDuration;
      inputfile >> tmp;
      inputfile >> parameter.nameOfOutputState;
      inputfile >> tmp;
      inputfile >> tmp;
      inputfile >> state.numberOfCycles;
      inputfile >> tmp;
      inputfile >> state.timeStepsPerCycle;
      inputfile >> tmp;
      inputfile >> tmp;
      inputfile >> parameter.epsilon;
      inputfile >> tmp;
      inputfile >> parameter.flagOfExactSol;
      inputfile >> tmp;
      inputfile >> tmp;
      if (mms.flagOfMms == 1 || mms.flagOfMms == 0 || mms.flagOfMms == 2)
          inputfile >> tmp;
      else
          inputfile >> mms.flagOfMms;
      if (mms.flagOfMms){
          inputfile >> tmp;
          inputfile >> parameter.nameOfOutputMms;
          inputfile >> tmp;
          inputfile >> timer.dt;
          inputfile >> tmp;
          inputfile >> storage.numberOfCells;
          inputfile >> tmp;
          inputfile >> storage.height;
          inputfile >> tmp;
          inputfile >> fluid.initVelocity;
          inputfile >> tmp;
          inputfile >> fluid.diffusivity;
          solid.diffusivity = fluid.diffusivity;
          inputfile >> tmp;
          inputfile >> fluid.inflowTemp;
          inputfile >> tmp;
          inputfile >> mms.n;
          inputfile >> tmp;
          inputfile >> mms.criterium;
      }

      inputfile.close();
  }
  else cout << "Unable to open file";
}

void
FILE_writeTemperature(void){
    FILE_setDividedOutputFileName();
    FILE_initializeTemperatureOutput();
    for (int i=0;i<storage.numberOfCells;i+=1)
        FILE_temperatureOutput(i);
}

void
FILE_initializeTemperatureOutput(void){
    ofstream outputfile;
    outputfile.open (parameter.nameOfDividedOutputTemperature, ios::trunc);
    outputfile << "Time: " << timer.time << " s" << endl;
    outputfile << "Fluid Velocity = " << fluid.velocity << endl;
    if (state.state == 0)
        outputfile << "State: Charging" << endl << endl;
    else if (state.state == 1)
        outputfile << "State: Discharging" <<  endl << endl;
    else
        outputfile << "State: Idle" <<  endl << endl;
    outputfile << "x LiquidTemp SolidTemp" << endl;
    outputfile.close();
}

void
FILE_temperatureOutput(int i){
    ofstream outputfile;
    outputfile.open (parameter.nameOfDividedOutputTemperature, ios::app);
    outputfile << cell.position[i] << " " << fluid.temperatureOld[i] << " " << solid.temperatureOld[i] << endl;
    outputfile.close();
}

void
FILE_setDividedOutputFileName(void){
    sprintf(parameter.nameOfDividedOutputTemperature,"%s%04d.prof", parameter.nameOfOutputTemperature, timer.iTimeStep);
}

void
FILE_initializeStateOutput(void){
    ofstream outputfile;
    outputfile.open (parameter.nameOfOutputState, ios::trunc);
    outputfile << "Time CycleTime State" << endl;
    outputfile.close();
}

void
FILE_stateOutput(void){
    ofstream outputfile;
    outputfile.open (parameter.nameOfOutputState, ios::app);
    outputfile << timer.time << " " << timer.timeInCycle << " " << state.state << endl;
    outputfile.close();
}

void
FILE_initializeLogFile(void){
    ofstream outputfile;
    outputfile.open (parameter.nameOfLogFile, ios::trunc);
    outputfile << "﻿###########-OutputLogFile-#########################################" << endl;
    if (mms.flagOfMms)
        outputfile << endl << "#=================>OrderVerificationCalculation<===================" << endl << endl;
    outputfile << "#--------InputData-------------------------------------------------" << endl;
    outputfile << "#--------StorageProperties-----------------------------------------" << endl;
    outputfile << "height(m)                                          " << storage.height << endl;
    outputfile << "diameter(m)                                        " << storage.diameter << endl;
    outputfile << "volume(m^3)                                        " << storage.volume << endl;
    outputfile << "numberOfCells                                      " << storage.numberOfCells << endl;
    outputfile << "CellLength(m)                                      " << storage.cellLength << endl;
    outputfile << "#--------FluidPhase------------------------------------------------" << endl;
    outputfile << "initialTemperature(K)                              " << fluid.initTemp << endl;
    outputfile << "inflowTemperature(K)                               " << fluid.inflowTemp << endl;
    outputfile << "diffusivity(m^2/s)                                 " << fluid.diffusivity << endl;
    outputfile << "velocity(m/s)                                      " << fluid.velocity << endl;
    outputfile << "massFlow(kg/s)                                     " << fluid.massFlow << endl;
    outputfile << "density(kg/m^3)                                    " << fluid.density << endl;
    outputfile << "specificHeatCapacity(J/kg*m)                       " << fluid.specHeatCap << endl;
    outputfile << "thermalConductivity(W/m*K)                         " << fluid.conductivity << endl;
    outputfile << "mu(kg/m*s)                                         " << fluid.mu << endl;
    outputfile << "#--------SolidPhase------------------------------------------------" << endl;
    outputfile << "initialTemperature(K)                              " << solid.initTemp << endl;
    outputfile << "diffusivity(m^2/s)                                 " << solid.diffusivity << endl;
    outputfile << "Density(kg/m^3)                                    " << solid.density << endl;
    outputfile << "diameter(m)                                        " << solid.diameter << endl;
    outputfile << "specificHeatCapacity(J/kg*m)                       " << solid.specHeatCap << endl;
    outputfile << "thermalConductivity(W/m*K)                         " << solid.conductivity << endl;
    outputfile << "#--------TemperatureOutput-----------------------------------------" << endl;
    outputfile << "nameOfTemperatureOutputFile                        " << parameter.nameOfOutputTemperature << endl;
    outputfile << "#--------Time------------------------------------------------------" << endl;
    outputfile << "finishTime(sec)                                    " << timer.finishTime << endl;
    outputfile << "timeDt(sec)                                        " << timer.dt << endl;
    outputfile << "#--------State-----------------------------------------------------" << endl;
    outputfile << "durationOfChargingState(h)                         " << state.chargingDuration << endl;
    outputfile << "durationOfFirstIdleState(h)                        " << state.firstIdleDuration << endl;
    outputfile << "durationOfDischargingState(h)                      " << state.dischargingDuration << endl;
    outputfile << "durationOfSecondIdleState(h)                       " << state.secondIdleDuration << endl;
    outputfile << "nameOfStateOutputFile                              " << parameter.nameOfOutputState << endl;
    outputfile << "#--------Cycle-----------------------------------------------------" << endl;
    outputfile << "numberOfCycles                                     " << state.numberOfCycles << endl;
    outputfile << "timeStepsPerCycle                                  " << state.timeStepsPerCycle << endl;
    outputfile << "#--------ConstParameters-------------------------------------------" << endl;
    outputfile << "epsilon                                            " << parameter.epsilon << endl;
    outputfile << "mmsCalculation                                     " << mms.flagOfMms << endl;
    if (mms.flagOfMms){
        outputfile << "nameOfMmsOutputFile                                " << parameter.nameOfOutputMms << endl;
        outputfile << "nOfMmsCalculation                                  " << mms.n << endl;
    }
    outputfile << endl << "﻿###########-CFLNumber-#############################################" << endl;
    outputfile << "CFLNumber                                          " << parameter.cfl << endl;
    outputfile << "CFL^2<CFL+2d<1?                                    " << parameter.cfl*parameter.cfl << "<" << parameter.cfl+2*parameter.d << "<1" << endl;
    if (parameter.cfl>1)
        outputfile << "Warning: CFL Number is larger than 1!" << endl;
    if (parameter.cfl*parameter.cfl>(parameter.cfl+2*parameter.d) || (parameter.cfl+2*parameter.d)>1)
        outputfile << "Warning: The condition CFL^2<CFL+2d<1 is not true!" << endl;
    if (parameter.cfl>1)
        cout << "Warning: CFL Number is larger than 1!" << endl;
    if (parameter.cfl*parameter.cfl>(parameter.cfl+2*parameter.d) || (parameter.cfl+2*parameter.d)>1)
        cout << "Warning: The condition CFL^2<CFL+2d<1 is not true!" << endl;
    outputfile << "﻿###########-HeatTransfer-##########################################" << endl;
    outputfile << "#--------CalculationOfHeatTransfer---------------------------------" << endl;
    outputfile << "pecletNumber                                       " << mms.peclet << endl;
    outputfile << "prandtlNumber                                      " << parameter.prandtl << endl;
    outputfile << "fluidVelocity                                      " << fluid.initVelocity << endl;
    outputfile << "reynoldsNumber                                     " << parameter.reynold << endl;
    outputfile << "nusseltNumber                                      " << parameter.nusselt << endl;
    outputfile << "fluidSolidHeatTransfer                             " << parameter.fsHeatTransfer << endl;
    outputfile << "heatTransferCoefficient                            " << parameter.heatTransferCoeff << endl;
    outputfile << "volumetricHeatTransferCoefficient                  " << parameter.volHeatTransferCoeff << endl;
    outputfile << "fluidHeatTransfer                                  " << fluid.heatTransfer << endl;
    outputfile << "solidHeatTransfer                                  " << solid.heatTransfer << endl << endl;
    outputfile << "﻿###########-Execution-#############################################" << endl;

    outputfile.close();
}

void
FILE_initializeMmsOutput(void){
    ofstream outputfile;
    outputfile.open (parameter.nameOfOutputMms, ios::trunc);
    outputfile << "dt = " << timer.dt << endl;
    outputfile << "dx = " << storage.cellLength << endl;
    outputfile << "fluidVelocity = " << fluid.velocity << endl;
    outputfile << "fluidDiffusivity = " << fluid.diffusivity << endl;
    outputfile << "inFlowTemp = " << fluid.inflowTemp << endl;
    outputfile << "pecletNumber = " << mms.peclet << endl;
    outputfile << "k = " << mms.k << endl << endl;
    outputfile << "time and error difference to last step:" << endl;
    outputfile.close();
}

void
FILE_mmsOutput(void){
    ofstream outputfile;
    outputfile.open (parameter.nameOfOutputMms, ios::app);
    outputfile << timer.time << " "  << mms.errorf << " " << fabs(mms.errorf - mms.errorfOld) << endl;
    outputfile.close();
}

void
FILE_mmsCheckFinalize(void){
    if (abs(mms.errorf - mms.errorfOld) < mms.criterium){
        if (!mms.finishf){
            FILE_mmsFinalF();
            cout << "execution time: " << TIMER_getWTime() - timer.t0 << " s" << endl;
            mms.finishf = 1;
        }
    }
    if (abs(mms.errors - mms.errorsOld) < mms.criterium){
        if (!mms.finishs){
            FILE_mmsFinalS();
            cout << "execution time: " << TIMER_getWTime() - timer.t0 << " s" << endl;
            mms.finishs = 1;
        }
    }
    if ((timer.iTimeStep == timer.finishTimeStep - 1) && (mms.finishf + mms.finishs != 2)){
        FILE_mmsFinalFail();
        cout << "execution time: " << TIMER_getWTime() - timer.t0 << " s" << endl;
        mms.finishf = 1;
        mms.finishs = 1;
    }

}

void
FILE_mmsFinalF(void){
    time_t t = time(0);
    struct tm * now = localtime( & t );

    ofstream outputfile;
    outputfile.open (parameter.nameOfFinalMms, ios::app);
    if (!mms.finishs){
        if (parameter.cfl>1)
            outputfile << "Warning: CFL Number is larger than 1!" << endl << endl;
        if (parameter.cfl*parameter.cfl>(parameter.cfl+2*parameter.d) || (parameter.cfl+2*parameter.d)>1)
            outputfile << "Warning: The condition CFL^2<CFL+2d<1 is not true!" << endl << endl;
        outputfile << "pecletNumber = " << mms.peclet << endl;
        outputfile << "dt = " << timer.dt << endl;
        outputfile << "dx = " << storage.cellLength << endl;
        outputfile << "fluidVelocity = " << fluid.velocity << endl;
        outputfile << "fluidDiffusivity = " << fluid.diffusivity << endl;
        outputfile << "solidDiffusivity = " << solid.diffusivity << endl;
        outputfile << "inFlowTemp = " << fluid.inflowTemp << endl;
        outputfile << "k = " << mms.k << endl;
        outputfile << "n = " << mms.n << endl;
    }
    outputfile << endl << "FluidError = " << mms.errorf << endl;
    outputfile << "log(FluidError) = " << log10(mms.errorf) << endl;
    outputfile << "time until error < " << mms.criterium << " : " << timer.time << " s" << endl;
    if (mms.finishs){
        outputfile << endl << "program finished " << now->tm_mday << "." << now->tm_mon << "." << now->tm_year + 1900 <<
            " at " << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << endl << endl;
        outputfile << "===================================================================" << endl << endl;
    }
    cout << "FinalF" << endl;
    outputfile.close();
}

void
FILE_mmsFinalS(void){
    time_t t = time(0);
    struct tm * now = localtime( & t );

    ofstream outputfile;
    outputfile.open (parameter.nameOfFinalMms, ios::app);
    if (!mms.finishf){
        if (parameter.cfl>1)
            outputfile << "Warning: CFL Number is larger than 1!" << endl << endl;
        if (parameter.cfl*parameter.cfl>(parameter.cfl+2*parameter.d) || (parameter.cfl+2*parameter.d)>1)
            outputfile << "Warning: The condition CFL^2<CFL+2d<1 is not true!" << endl << endl;
        outputfile << "pecletNumber = " << mms.peclet << endl;
        outputfile << "dt = " << timer.dt << endl;
        outputfile << "dx = " << storage.cellLength << endl;
        outputfile << "fluidVelocity = " << fluid.velocity << endl;
        outputfile << "fluidDiffusivity = " << fluid.diffusivity << endl;
        outputfile << "solidDiffusivity = " << solid.diffusivity << endl;
        outputfile << "inFlowTemp = " << fluid.inflowTemp << endl;
        outputfile << "k = " << mms.k << endl;
        outputfile << "n = " << mms.n << endl;
    }
    outputfile << endl << "SolidError = " << mms.errors << endl;
    outputfile << "log(SolidError) = " << log10(mms.errors) << endl;
    outputfile << "time until error < " << mms.criterium << " : " << timer.time << " s" << endl;
    if (mms.finishf){
        outputfile << endl << "program finished " << now->tm_mday << "." << now->tm_mon << "." << now->tm_year + 1900 <<
            " at " << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << endl << endl;
        outputfile << "===================================================================" << endl << endl;
    }
    cout << "FinalS" << endl;
    outputfile.close();
}

void
FILE_mmsFinalFail(void){
    time_t t = time(0);
    struct tm * now = localtime( & t );

    ofstream outputfile;
    outputfile.open (parameter.nameOfFinalMms, ios::app);
    outputfile << endl;
    if (mms.finishf + mms.finishs == 0){
        if (parameter.cfl>1)
            outputfile << "Warning: CFL Number is larger than 1!" << endl << endl;
        if (parameter.cfl*parameter.cfl>(parameter.cfl+2*parameter.d) || (parameter.cfl+2*parameter.d)>1)
            outputfile << "Warning: The condition CFL^2<CFL+2d<1 is not true!" << endl << endl;
        outputfile << "pecletNumber = " << mms.peclet << endl;
        outputfile << "dt = " << timer.dt << endl;
        outputfile << "dx = " << storage.cellLength << endl;
        outputfile << "fluidVelocity = " << fluid.velocity << endl;
        outputfile << "fluidDiffusivity = " << fluid.diffusivity << endl;
        outputfile << "solidDiffusivity = " << solid.diffusivity << endl;
        outputfile << "inFlowTemp = " << fluid.inflowTemp << endl;
        outputfile << "k = " << mms.k << endl;
        outputfile << "n = " << mms.n << endl;

    }
    cout << "FinalFail" << endl;
    if (!mms.finishf){
        outputfile << endl << "Warning: The fluid error difference never became smaller than " << mms.criterium << "," << endl;
        outputfile << "the smallest error difference was " << fabs(mms.errorf - mms.errorfOld) << endl << endl;
        cout << "Warning: The fluid error difference never became smaller than " << mms.criterium << "," << endl;
        cout << "the smallest error difference was " << fabs(mms.errorf - mms.errorfOld) << endl << endl;
        outputfile << "FluidError = " << mms.errorf << endl;
        outputfile << "log(FluidError) = " << log10(mms.errorf) << endl;
    }
    if (!mms.finishs){
        outputfile << endl << "Warning: The solid error difference never became smaller than " << mms.criterium << "," << endl;
        outputfile << "the smallest error difference was " << fabs(mms.errors - mms.errorsOld) << endl << endl;
        cout << "Warning: The solid error difference never became smaller than " << mms.criterium << "," << endl;
        cout << "the smallest error difference was " << fabs(mms.errors - mms.errorsOld) << endl << endl;
        outputfile << "SolidError = " << mms.errors << endl;
        outputfile << "log(SolidError) = " << log10(mms.errors) << endl;
    }
    outputfile << endl << "program finished " << now->tm_mday << "." << now->tm_mon << "." << now->tm_year + 1900 <<
        " at " << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << endl << endl;
    outputfile << "===================================================================" << endl << endl;
    outputfile.close();
}

void
FILE_initializeResults(){
    ifstream ifile(parameter.nameOfResults);
    //if (!ifile) {
        ofstream outputfile;
        outputfile.open (parameter.nameOfResults, ios::trunc);
        outputfile << "a b c d" << endl;
        outputfile.close();
    //}
}

void
FILE_writeResults(){
    ofstream outputfile;
    outputfile.open(parameter.nameOfResults, ios::app);
    outputfile << storage.diameter << " " << fluid.tempIncrease << " ";
    outputfile << energy.efficiency << " " << energy.capacityFactor << endl;
    outputfile.close();
}
