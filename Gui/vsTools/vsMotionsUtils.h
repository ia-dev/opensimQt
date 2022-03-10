/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                          *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#ifndef VSMOTIONSUTILS_H
#define VSMOTIONSUTILS_H

#include <QMap>
#include <QObject>
#include "OpenSim.h"

/// type of operation associated with a Motion-Model pair. for now we only use it for Opening
enum MotionOperation{Open, Close, CurrentMotionsChanged, Modified, Assoc};

/**
 * struct that store the parameter for motion event, @see MotionOperation
 */
struct MotionEventObject
{
    OpenSim::Model *m_model;
    OpenSim::Storage *m_storage;
    MotionOperation m_motionOperation;
    std::string m_FilePath;
    MotionEventObject(OpenSim::Model* model,OpenSim::Storage *storage,MotionOperation motionOperation, std::string FilePath )
        :m_model(model),m_storage(storage),m_motionOperation(motionOperation),m_FilePath(FilePath ) {}
};
typedef MotionEventObject MotionEventObject;


/**
 * The single source class that manage motion.
 * it should be instanciated one time, with the instance static variable as a way to do motion operations throught the application.
 * @see instance
 */
class vsMotionsUtils : public QObject
{
    Q_OBJECT
public:
    explicit vsMotionsUtils(QObject *parent = nullptr);
    /// the main vsMotionsUtils instace.
    static vsMotionsUtils *instance;
    /// get the instance pointer and insure its initiation.
    static vsMotionsUtils* getInstance();

    /// the model that all current motions operation will be applied to.
    OpenSim::Model *activeModel = nullptr;

    /// link each model with the Motions applied to it
    QMap<OpenSim::Model*,QList<OpenSim::Storage*>*> mapModelsToMotions;

    /// @deprecated
    QMap<OpenSim::Storage*,QBitArray*> mapMotionToBitArray;

    /// the currunt motion-model pair
    QPair<OpenSim::Model*,OpenSim::Storage*> *currentMotion;

    /// the manager of the simulation, all the simulation parameters will be feed to this manager.
    OpenSim::Manager *currentManager;


    //motion functions
    /**
     * opens the dialog to select a motion file and then apply it to the correspending model.
     * it sets the currentModel to the model
     * @param model the model that the storage will be loaded to and will be set as the currentModel.
     * @see currentModel
     * @see loadMotionStorage
     */
    void openLoadMotionDialog(OpenSim::Model *model = nullptr);
    /// @skip no functionality yet (maybe the code from other functions will be refactored to it) @see loadMotionStorage
    void loadMotionFile(std::string fileName);
    /**
     * loads the given storage to the currentModel using the addMotion fuction.
     * @param newMotion motion to be loaded
     * @param primary not usefull yet (prototype from OpenSim GUI)
     * @param filePath not usefull yet (prototype form OpenSim GUI)
     * @see addMotion
     */
    void loadMotionStorage(OpenSim::Storage *newMotion,bool primary,std::string filePath);
    /**
     * check if a model is compatible with a motion.
     * @param model
     * @param storage
     * @return wether the model is compatible (true) or not (false)
     */
    bool testMotionAssociationPossible(OpenSim::Model *model,OpenSim::Storage *storage);
    /**
     * finalize the linkage of Model-Motion.
     * update the current motion with the new pair.
     * it sends notifyObesrver signal with a filled MotionEventObject to inform that the currentMotion has changed.
     * @param model
     * @param newMotion
     * @param parentMotion left nullptr for now.
     * @see mapModelsToMotions
     * @see MotionEventObject
     * @see notifyObservers
     */
    void addMotion(OpenSim::Model *model , OpenSim::Storage *newMotion , OpenSim::Storage *parentMotion, std::string strFilePath);
    /**
     * set the current motion-model pair and set the state of the model the the first frame.
     * @param model
     * @param motion
     * @see applyTimeToModel
     * @see applyFrameToModel
     */
    void setCurrentMotion(OpenSim::Model *model,OpenSim::Storage *motion);
    /**
     * apply the given time to the model from the storage.
     * it took the first frame in the storage in an the time->time+30ms interval.
     * it set the visualizerVTK::updating member to true to not load the opensim model again and again.
     * after that it update all the decorations in that time.
     * @param model
     * @param motion
     * @param time the time to set the state to.
     * @see vsNavigatorNode::updating
     * @see vsNavigatorNode::updateModelDecoration
     */
    void applyTimeToModel(OpenSim::Model *model,OpenSim::Storage *motion,double time);
    /**
     * same as applyTimeToModel but this function use the number of frame inside the motion, instead of time
     * @param model
     * @param motion
     * @param framNumber
     */
    void applyFrameToModel(OpenSim::Model *model, OpenSim::Storage *motion,int framNumber);

    //simulation functions
    /**
     * get the current model and apply simulation calculation over it.
     * @param endTime the stopping time of the simulation
     * @see vsNavigatorNode::getNavigatorModel
     * @see vsNavigatorModel::getActiveModel
     * @see applySimulationToModel
     */
    void applySimulationToCurrentModel(double endTime);
    /**
     * check the valididty of the model and motion and then apply the simulation using configurable manager.
     * @param endTime stopping time of the simulation
     * @param accuracy  degree of results accuracy
     * @param stepSize the diffrance in time between each frame
     * @param integrator the type of integrator used in the simulation
     * @see applySimulationToModelUsingManager
     */
    void applySimulationToCurrentModelM(double endTime,double accuracy, double stepSize,
                                        OpenSim::Manager::IntegratorMethod integrator);
    /**
     * apply simulation to a model using the OpenSim::simulate.
     * @param model
     * @param endTime
     * @see applyTimeToSimulation
     * @see applyTimeToModelUsingManager
     */
    void applySimulationToModel(OpenSim::Model *model,double endTime);
    /**
     * apply a simulation to a model using the Manager::integrate.
     * it also load the results to the given model .setup the motions and log the integration results.
     * @param model
     * @param endTime
     * @param accuracy
     * @param stepSize
     * @param integrator
     * @see applySimulationToCurrentModelM
     * @see currentManager
     */
    void applySimulaitonToModelUsingManager(OpenSim::Model *model, double endTime,double accuracy,
                                            double stepSize,OpenSim::Manager::IntegratorMethod integrator);

public slots:
    /// listen to the notifyObserves signal to update the currentMotion with the MotionEventObject.
    /// @see MotionEventObject
    void update(MotionEventObject eventObj);
signals:
    /// signal emited when a new motion event is present, like opening a new motion file and load it to the model.
    /// @see MotionEventObject
    void notifyObservers(MotionEventObject eventObj);
    /// notification of currentMotion change signal.
    void currentMotionChanged();


};

#endif // VSMOTIONSUTILS_H
