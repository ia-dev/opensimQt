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

enum MotionOperation{Open, Close, CurrentMotionsChanged, Modified, Assoc};

struct MotionEventObject
{
    OpenSim::Model *m_model;
    OpenSim::Storage *m_storage;
    MotionOperation m_motionOperation;
    MotionEventObject(OpenSim::Model* model,OpenSim::Storage *storage,MotionOperation motionOperation)
        :m_model(model),m_storage(storage),m_motionOperation(motionOperation) {}
};
typedef MotionEventObject MotionEventObject;

class vsMotionsUtils : public QObject
{
    Q_OBJECT
public:
    explicit vsMotionsUtils(QObject *parent = nullptr);
    static vsMotionsUtils *instance;
    static vsMotionsUtils* getInstance();
    //
    OpenSim::Model *activeModel = nullptr;
    QMap<OpenSim::Model*,QList<OpenSim::Storage*>*> mapModelsToMotions;
    QMap<OpenSim::Storage*,QBitArray*> mapMotionToBitArray;
    QPair<OpenSim::Model*,OpenSim::Storage*> *currentMotion;


    //motion functions
    void openLoadMotionDialog(OpenSim::Model *model = nullptr);
    void loadMotionFile(std::string fileName);
    void loadMotionStorage(OpenSim::Storage *newMotion,bool primary,std::string filePath);
    bool testMotionAssociationPossible(OpenSim::Model *model,OpenSim::Storage *storage);
    void addMotion(OpenSim::Model *model , OpenSim::Storage *newMotion , OpenSim::Storage *parentMotion);
    void setCurrentMotion(OpenSim::Model *model,OpenSim::Storage *motion);
    void applyTimeToModel(OpenSim::Model *model,OpenSim::Storage *motion,double time);

public slots:
    void update(MotionEventObject eventObj);
signals:
    void notifyObservers(MotionEventObject eventObj);


};

#endif // VSMOTIONSUTILS_H
