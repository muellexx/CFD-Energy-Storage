#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED


void
FILE_readDataFile(void);

void
FILE_writeTemperature(void);

void
FILE_initializeTemperatureOutput(void);

void
FILE_temperatureOutput(int i);

void
FILE_setDividedOutputFileName(void);

void
FILE_initializeLogFile(void);

void
FILE_initializeStateOutput(void);

void
FILE_stateOutput(void);

void
FILE_writeMms(void);

void
FILE_setDividedMmsOutputFileName(void);

void
FILE_initializeMmsOutput(void);

void
FILE_mmsOutput(void);

void
FILE_mmsCheckFinalize(void);

void
FILE_mmsFinalS(void);

void
FILE_mmsFinalF(void);

void
FILE_mmsFinalFail(void);

void
FILE_initializeResults();

void
FILE_writeResults();


#endif // FILE_H_INCLUDED
