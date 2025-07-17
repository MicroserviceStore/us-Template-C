/*
 * This header is an interface between the Native Code and Simulation in The Windows Form Environment
 */
#ifndef __SIMULATION_H
#define __SIMULATION_H

#include <stdint.h>

/*
 * Simulation Details
 */
typedef struct
{
    #define SIM_CPUCORE_NAME_LENGTH         16

    /* We use this name to simulate the CPU Core Specific behaviour in simulation level */
    char simCPUCoreName[SIM_CPUCORE_NAME_LENGTH];

    /* Each Simulation shall have an index */
    uint8_t simExecIndex;
} Sys_SimulationDetails;

/* LOG Callback to pass the Logs from Native Code to the Windows Form Log */
typedef void(*LOGCB)(uint8_t*);
void SIM_RegisterLOGCallback(LOGCB logCB);

/* Simulation Init */
void Sys_SimulationInit(Sys_SimulationDetails* simDetails);

/*
 * The Native Application Entry Point
 */
extern int main(void);

#endif
