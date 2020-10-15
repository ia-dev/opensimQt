#ifndef VSCOMMON_H
#define VSCOMMON_H

typedef struct SimulationParams {
    unsigned short flag : 8;
    int integratorMethod;
    double accuracy;
    double stepSize;
    double endTime;
} SimulationParams;

enum SimParamFlag {
    None = 0,
    Accuracy = 1,
    StepSize = 2,
    EndTime = 4,
    IntegratorMethod = 8
};

#endif // VSCOMMON_H
