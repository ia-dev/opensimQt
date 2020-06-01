/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                          *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#ifndef VSMOTIONSUTILS_H
#define VSMOTIONSUTILS_H

#include <QObject>
#include "OpenSim.h"

class vsMotionsUtils : public QObject
{
    Q_OBJECT
public:
    explicit vsMotionsUtils(QObject *parent = nullptr);
    static vsMotionsUtils *instance;
    static vsMotionsUtils* getInstance();

    //motion functions
    void openLoadMotionDialog(OpenSim::Model *model = nullptr);
    void loadMotionFile(std::string fileName);
    void loadMotionStorage(OpenSim::Storage *newMotion,bool primary,std::string filePath);
    bool testMotionAssociationPossible(OpenSim::Model *model,OpenSim::Storage *storage);
    void addMotion(OpenSim::Model *model , OpenSim::Storage *newMotion , OpenSim::Storage *parentMotion);

signals:

};

#endif // VSMOTIONSUTILS_H
