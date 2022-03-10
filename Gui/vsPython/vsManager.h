#ifndef VSMANAGER_H
#define VSMANAGER_H

#include <QObject>
#include <vsCommon/vsCommon.h>

class vsManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(IntegratorMethod)

public:
    vsManager(QObject* parent=nullptr);
    virtual ~vsManager() {}


    /**
     *  @brief Integrator Method : Enumerates supported Integrator Methods
     */
    enum class IntegratorMethod {
        ExplicitEuler      = 0, ///< 0 : For details, see SimTK::ExplicitEulerIntegrator.
        RungeKutta2        = 1, ///< 1 : For details, see SimTK::RungeKutta2Integrator.
        RungeKutta3        = 2, ///< 2 : For details, see SimTK::RungeKutta3Integrator.
        RungeKuttaFeldberg = 3, ///< 3 : For details, see SimTK::RungeKuttaFeldbergIntegrator.
        RungeKuttaMerson   = 4, ///< 4 : For details, see SimTK::RungeKuttaMersonIntegrator.
        SemiExplicitEuler2 = 5, ///< 5 : For details, see SimTK::SemiExplicitEuler2Integrator.
        Verlet             = 6  ///< 6 : For details, see SimTK::VerletIntegrator.

    };


signals:
    void updateSimulationParams(const SimulationParams p);

public slots:
    /**
     * @brief Sets the accuracy parameter of Integrator
     * @param accuracy Accuracy expressed as a real number
     */
    void setAccuracy(double accuracy);
    /**
     * @brief Sets the maximum step size of the Integrator
     * @param stepSize Step Size as a real number
     */
    void setStepSize(double stepSize);
    /**
     * @brief Sets the end time of the simulation
     * @param endTime End Time as real number
     */
    void setEndTime(double endTime);
    /**
     * @brief Sets the integrator method to be used with the help of IntegratorMethod enum
     * @param integratorMethod
     *
     * OpenSim::Manager::IntegratorMethod enum holds a set of constants representing different
     * Integrator Methods supported by SimTK.
     */
    void setIntegratorMethod(vsManager::IntegratorMethod integratorMethod);
    /**
     * @brief Updates the GUI with set simulation parameters
     */
    void updSimDataToGui();

    /**
     * @brief Runs a Simulation on current model
     */
    void runSimulation();
    /**
     * @brief Stops the running Simulation
     */
    void stopSimulation();

private:
    //data structure that holds the values of all parameters
    SimulationParams sp;

};



#endif // VSMANAGER_H
