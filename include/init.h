#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED


void
INIT_initializeParameters(int argumentCount,char **argumentVector);

void
INIT_initializeTemperature(void);

void
INIT_calculateVelocity(void);

void
INIT_checkCFLNumber(void);

void
INIT_initializeHeatTransfer(void);

void
INIT_initializeMms(void);


#endif // INIT_H_INCLUDED
