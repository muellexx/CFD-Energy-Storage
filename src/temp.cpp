#include <iostream>
#include <math.h>
#include <omp.h>

#include "struct.h"
#include "temp.h"

# define PI           3.14159265358979323846  /* pi */

using namespace std;


void
TEMP_FTBSSimplified(void){
    int n = storage.numberOfCells - 1;
    int i;

    fluid.temperatureNew[0] = fluid.temperatureOld[0]  - fluid.velocity*timer.dt*(fluid.temperatureOld[0]-fluid.inflowTemp)/storage.cellLength
            + fluid.diffusivity*timer.dt*(fluid.temperatureOld[1]-fluid.temperatureOld[0])/(storage.cellLength*storage.cellLength);
    solid.temperatureNew[0] = solid.temperatureOld[0]
            + solid.diffusivity*timer.dt*(solid.temperatureOld[1] - solid.temperatureOld[0])/(storage.cellLength*storage.cellLength);
    fluid.temperatureNew[n] = fluid.temperatureOld[n] - fluid.velocity*timer.dt*(fluid.temperatureOld[n]-fluid.temperatureOld[n-1])/storage.cellLength
            + fluid.diffusivity*timer.dt*(fluid.temperatureOld[n-1]-fluid.temperatureOld[n])/(storage.cellLength*storage.cellLength);
    solid.temperatureNew[n] = solid.temperatureOld[n]
            + solid.diffusivity*timer.dt*(solid.temperatureOld[n-1] - solid.temperatureOld[n])/(storage.cellLength*storage.cellLength);

    for (i=1; i<n; i++){
        fluid.temperatureNew[i] = fluid.temperatureOld[i] - fluid.velocity*timer.dt*(fluid.temperatureOld[i]-fluid.temperatureOld[i-1])/storage.cellLength
            + fluid.diffusivity*timer.dt*(fluid.temperatureOld[i+1]-2*fluid.temperatureOld[i]+fluid.temperatureOld[i-1])/(storage.cellLength*storage.cellLength);
        solid.temperatureNew[i] = solid.temperatureOld[i]
            + solid.diffusivity*timer.dt*(solid.temperatureOld[i+1]-2*solid.temperatureOld[i]+solid.temperatureOld[i-1])/(storage.cellLength*storage.cellLength);
    }
    for (i=0; i<n+1;i++){
        fluid.temperatureOld[i]=fluid.temperatureNew[i];
        solid.temperatureOld[i]=solid.temperatureNew[i];
    }
}

void
TEMP_FTBS(void){
    if (state.state == 1)
        TEMP_FTBSDischarging();
    else
        TEMP_FTBSCharging();
}

void
TEMP_FTBSCharging(void){
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
    solidTempStar = solid.temperatureOld[0]
            + solid.diffusivity*timer.dt*(solid.temperatureOld[1] - solid.temperatureOld[0])/(storage.cellLength*storage.cellLength);
    fluid.temperatureNew[0] = e*(d*fluidTempStar-b*solidTempStar);
    solid.temperatureNew[0] = e*(a*solidTempStar-c*fluidTempStar);
    fluidTempStar = fluid.temperatureOld[n] - fluid.velocity*timer.dt*(fluid.temperatureOld[n]-fluid.temperatureOld[n-1])/storage.cellLength
            + fluid.diffusivity*timer.dt*(fluid.temperatureOld[n-1]-fluid.temperatureOld[n])/(storage.cellLength*storage.cellLength);
    solidTempStar = solid.temperatureOld[n]
            + solid.diffusivity*timer.dt*(solid.temperatureOld[n-1] - solid.temperatureOld[n])/(storage.cellLength*storage.cellLength);
    fluid.temperatureNew[n] = e*(d*fluidTempStar-b*solidTempStar);
    solid.temperatureNew[n] = e*(a*solidTempStar-c*fluidTempStar);

    omp_set_num_threads(parameter.threads);
#pragma omp parallel for private(fluidTempStar,solidTempStar)
    for (i=1; i<n; i++){
        fluidTempStar = fluid.temperatureOld[i] - fluid.velocity*timer.dt*(fluid.temperatureOld[i]-fluid.temperatureOld[i-1])/storage.cellLength
            + fluid.diffusivity*timer.dt*(fluid.temperatureOld[i+1]-2*fluid.temperatureOld[i]+fluid.temperatureOld[i-1])/(storage.cellLength*storage.cellLength);
        solidTempStar = solid.temperatureOld[i]
            + solid.diffusivity*timer.dt*(solid.temperatureOld[i+1]-2*solid.temperatureOld[i]+solid.temperatureOld[i-1])/(storage.cellLength*storage.cellLength);
        fluid.temperatureNew[i] = e*(d*fluidTempStar-b*solidTempStar);
        solid.temperatureNew[i] = e*(a*solidTempStar-c*fluidTempStar);
    }

    omp_set_num_threads(parameter.threads);
#pragma omp parallel for
    for (i=0; i<n+1;i++){
        fluid.temperatureOld[i]=fluid.temperatureNew[i];
        solid.temperatureOld[i]=solid.temperatureNew[i];
    }


}

void
TEMP_FTBSDischarging(void){
    int n = storage.numberOfCells - 1;
    int i;
    double fluidTempStar;
    double solidTempStar;
    double a, b, c, d;
    a=1+fluid.heatTransfer*timer.dt;
    b=-fluid.heatTransfer*timer.dt;
    c=-solid.heatTransfer*timer.dt;
    d=1+solid.heatTransfer*timer.dt;
    double e = 1/(a*d-b*c);

    fluidTempStar = fluid.temperatureOld[0] - fluid.velocity*timer.dt*(fluid.temperatureOld[0]-fluid.temperatureOld[1])/storage.cellLength
            + fluid.diffusivity*timer.dt*(fluid.temperatureOld[1]-fluid.temperatureOld[0])/(storage.cellLength*storage.cellLength);
    solidTempStar = solid.temperatureOld[0]
            + solid.diffusivity*timer.dt*(solid.temperatureOld[1] - solid.temperatureOld[0])/(storage.cellLength*storage.cellLength);
    fluid.temperatureNew[0] = e*(d*fluidTempStar-b*solidTempStar);
    solid.temperatureNew[0] = e*(a*solidTempStar-c*fluidTempStar);
    fluidTempStar = fluid.temperatureOld[n] - fluid.velocity*timer.dt*(fluid.temperatureOld[n]-fluid.initTemp)/storage.cellLength
            + fluid.diffusivity*timer.dt*(fluid.temperatureOld[n-1] - fluid.temperatureOld[n])/(storage.cellLength*storage.cellLength);
    solidTempStar = solid.temperatureOld[n]
            + solid.diffusivity*timer.dt*(solid.temperatureOld[n-1] - solid.temperatureOld[n])/(storage.cellLength*storage.cellLength);
    fluid.temperatureNew[n] = e*(d*fluidTempStar-b*solidTempStar);
    solid.temperatureNew[n] = e*(a*solidTempStar-c*fluidTempStar);

    omp_set_num_threads(parameter.threads);
#pragma omp parallel for private(fluidTempStar,solidTempStar)
    for (i=1; i<n; i++){
        fluidTempStar = fluid.temperatureOld[i] - fluid.velocity*timer.dt*(fluid.temperatureOld[i]-fluid.temperatureOld[i+1])/storage.cellLength
            + fluid.diffusivity*timer.dt*(fluid.temperatureOld[i+1]-2*fluid.temperatureOld[i]+fluid.temperatureOld[i-1])/(storage.cellLength*storage.cellLength);
        solidTempStar = solid.temperatureOld[i]
            + solid.diffusivity*timer.dt*(solid.temperatureOld[i+1]-2*solid.temperatureOld[i]+solid.temperatureOld[i-1])/(storage.cellLength*storage.cellLength);
        fluid.temperatureNew[i] = e*(d*fluidTempStar-b*solidTempStar);
        solid.temperatureNew[i] = e*(a*solidTempStar-c*fluidTempStar);
    }

    omp_set_num_threads(parameter.threads);
#pragma omp parallel for
    for (i=0; i<n+1;i++){
        fluid.temperatureOld[i]=fluid.temperatureNew[i];
        solid.temperatureOld[i]=solid.temperatureNew[i];
    }
}
