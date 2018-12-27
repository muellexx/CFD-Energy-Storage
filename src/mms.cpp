#include <iostream>
#include <math.h>
#include <omp.h>

#include "struct.h"
#include "file.h"
#include "mms.h"

# define PI           3.14159265358979323846  /* pi */


void
MMS_Calculate(void){
    if (mms.flagOfMms == 1)
        MMS_FTBSSimplified();
    else if (mms.flagOfMms == 2)
        MMS_FTBSCharging();
}

void
MMS_FTBSSimplified(void){
    int n = storage.numberOfCells - 1;
    int i;
    if (timer.iTimeStep==1)

    fluid.temperatureNew[0] = fluid.temperatureOld[0]  - fluid.velocity*timer.dt*(fluid.temperatureOld[0]-fluid.inflowTemp)/storage.cellLength
            + fluid.diffusivity*timer.dt*(fluid.temperatureOld[1]-fluid.temperatureOld[0])/(storage.cellLength*storage.cellLength)
            + timer.dt*mms.sManf[0];
    solid.temperatureNew[0] = solid.temperatureOld[0]
            + solid.diffusivity*timer.dt*(solid.temperatureOld[1] - solid.temperatureOld[0])/(storage.cellLength*storage.cellLength)
            + timer.dt*mms.sMans[0];
    fluid.temperatureNew[n] = fluid.temperatureOld[n] - fluid.velocity*timer.dt*(fluid.temperatureOld[n]-fluid.temperatureOld[n-1])/storage.cellLength
            + fluid.diffusivity*timer.dt*(fluid.temperatureOld[n-1]-fluid.temperatureOld[n])/(storage.cellLength*storage.cellLength)
            + timer.dt*mms.sManf[n];
    solid.temperatureNew[n] = solid.temperatureOld[n]
            + solid.diffusivity*timer.dt*(solid.temperatureOld[n-1] - solid.temperatureOld[n])/(storage.cellLength*storage.cellLength)
            + timer.dt*mms.sMans[n];

    for (i=1; i<n; i++){
        fluid.temperatureNew[i] = fluid.temperatureOld[i] - fluid.velocity*timer.dt*(fluid.temperatureOld[i]-fluid.temperatureOld[i-1])/storage.cellLength
            + fluid.diffusivity*timer.dt*(fluid.temperatureOld[i+1]-2*fluid.temperatureOld[i]+fluid.temperatureOld[i-1])/(storage.cellLength*storage.cellLength)
            + timer.dt*mms.sManf[i];
        solid.temperatureNew[i] = solid.temperatureOld[i]
            + solid.diffusivity*timer.dt*(solid.temperatureOld[i+1]-2*solid.temperatureOld[i]+solid.temperatureOld[i-1])/(storage.cellLength*storage.cellLength)
            + timer.dt*mms.sMans[i];
    }
    for (i=0; i<n+1;i++){
        fluid.temperatureOld[i]=fluid.temperatureNew[i];
        solid.temperatureOld[i]=solid.temperatureNew[i];
    }

    mms.errorfOld = mms.errorf;
    mms.errorsOld = mms.errors;
    if (!mms.finishf)
        mms.errorf = 0.;
    if (!mms.finishs)
        mms.errors = 0.;
    for (int i=0; i<storage.numberOfCells; i++){
        if (!mms.finishf)
            mms.errorf += fabs(fluid.temperatureOld[i] - cos(mms.k*cell.position[i]));
        if (!mms.finishs)
            mms.errors += fabs(solid.temperatureOld[i] - cos(mms.k*cell.position[i]));
    }
    if (!mms.finishf)
        mms.errorf=mms.errorf/storage.numberOfCells;
    if (!mms.finishs)
        mms.errors=mms.errors/storage.numberOfCells;
    FILE_mmsOutput();
}

void
MMS_FTBSCharging(void){
    int n = storage.numberOfCells - 1;
    int i;
    double fluidTempStar;
    double solidTempStar;
    double a, b, c, d;
    a=1+fluid.heatTransfer*timer.dt;
    b= 0-fluid.heatTransfer*timer.dt;
    c= 0-solid.heatTransfer*timer.dt;
    d=1+solid.heatTransfer*timer.dt;
    double e = 1/(a*d-b*c);

    fluidTempStar = fluid.temperatureOld[0] - fluid.velocity*timer.dt*(fluid.temperatureOld[0]-fluid.inflowTemp)/storage.cellLength
            + fluid.diffusivity*timer.dt*(fluid.temperatureOld[1]-fluid.temperatureOld[0])/(storage.cellLength*storage.cellLength);
            //+ timer.dt*mms.sManf[0];
    solidTempStar = solid.temperatureOld[0]
            + solid.diffusivity*timer.dt*(solid.temperatureOld[1] - solid.temperatureOld[0])/(storage.cellLength*storage.cellLength);
            //+ timer.dt*mms.sMans[0];
    fluid.temperatureNew[0] = e*(d*fluidTempStar-b*solidTempStar) + timer.dt*mms.sManf[0];
    solid.temperatureNew[0] = e*(a*solidTempStar-c*fluidTempStar) + timer.dt*mms.sMans[0];
    fluidTempStar = fluid.temperatureOld[n] - fluid.velocity*timer.dt*(fluid.temperatureOld[n]-fluid.temperatureOld[n-1])/storage.cellLength
            + fluid.diffusivity*timer.dt*(fluid.temperatureOld[n-1]-fluid.temperatureOld[n])/(storage.cellLength*storage.cellLength);
            //+ timer.dt*mms.sManf[n];
    solidTempStar = solid.temperatureOld[n]
            + solid.diffusivity*timer.dt*(solid.temperatureOld[n-1] - solid.temperatureOld[n])/(storage.cellLength*storage.cellLength);
            //+ timer.dt*mms.sMans[n];
    fluid.temperatureNew[n] = e*(d*fluidTempStar-b*solidTempStar) + timer.dt*mms.sManf[n];
    solid.temperatureNew[n] = e*(a*solidTempStar-c*fluidTempStar) + timer.dt*mms.sMans[n];

    omp_set_num_threads(parameter.threads);
#pragma omp parallel for private(fluidTempStar,solidTempStar)
    for (i=1; i<n; i++){
        fluidTempStar = fluid.temperatureOld[i] - fluid.velocity*timer.dt*(fluid.temperatureOld[i]-fluid.temperatureOld[i-1])/storage.cellLength
            + fluid.diffusivity*timer.dt*(fluid.temperatureOld[i+1]-2*fluid.temperatureOld[i]+fluid.temperatureOld[i-1])/(storage.cellLength*storage.cellLength);
            //+ timer.dt*mms.sManf[i];
        solidTempStar = solid.temperatureOld[i]
            + solid.diffusivity*timer.dt*(solid.temperatureOld[i+1]-2*solid.temperatureOld[i]+solid.temperatureOld[i-1])/(storage.cellLength*storage.cellLength);
            //+ timer.dt*mms.sMans[i];
        fluid.temperatureNew[i] = e*(d*fluidTempStar-b*solidTempStar) + timer.dt*mms.sManf[i];
        solid.temperatureNew[i] = e*(a*solidTempStar-c*fluidTempStar) + timer.dt*mms.sMans[i];
    }

    omp_set_num_threads(parameter.threads);
#pragma omp parallel for
    for (i=0; i<n+1;i++){
        fluid.temperatureOld[i]=fluid.temperatureNew[i];
        solid.temperatureOld[i]=solid.temperatureNew[i];
    }

    mms.errorfOld = mms.errorf;
    mms.errorsOld = mms.errors;
    if (!mms.finishf)
        mms.errorf = 0.;
    if (!mms.finishs)
        mms.errors = 0.;
    for (int i=0; i<storage.numberOfCells; i++){
        if (!mms.finishf)
            mms.errorf += fabs(fluid.temperatureOld[i] - cos(mms.k*cell.position[i]));
        if (!mms.finishs)
            mms.errors += fabs(solid.temperatureOld[i] - cos(mms.k*cell.position[i]));
    }
    if (!mms.finishf)
        mms.errorf=mms.errorf/storage.numberOfCells;
    if (!mms.finishs)
        mms.errors=mms.errors/storage.numberOfCells;
    FILE_mmsOutput();

}
