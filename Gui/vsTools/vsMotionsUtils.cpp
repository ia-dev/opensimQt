/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                          *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#include "vsMotionsUtils.h"

#include <QFileDialog>
#include "vsOpenSimTools.h"

vsMotionsUtils* vsMotionsUtils::instance = nullptr;

vsMotionsUtils::vsMotionsUtils(QObject *parent) : QObject(parent)
{

}

vsMotionsUtils *vsMotionsUtils::getInstance()
{
    if(!instance) instance = new vsMotionsUtils();
    return instance;
}

void vsMotionsUtils::openLoadMotionDialog(OpenSim::Model *model)
{
    try {
        QString motionFile = QFileDialog::getOpenFileName(nullptr,"Load Motion File To Current Model","","Motions (*.mot *.sto)");
        if(motionFile == "") throw  QString("error");

        OpenSim::Storage *storage = new OpenSim::Storage(motionFile.toStdString());
        // testing the possibility to associate the motion
        vsOpenSimTools::tools->log("motion file selected :"+motionFile,"vsModelNode");
    }
    catch (QString s) {
        vsOpenSimTools::tools->log("no file was selected :","vsModelNode",vsOpenSimTools::Error);
    }
    catch(...){
        vsOpenSimTools::tools->log("loading motion file failed :","vsModelNode",vsOpenSimTools::Error);
    }
}

void vsMotionsUtils::loadMotionFile(std::string fileName)
{

}

void vsMotionsUtils::loadMotionStorage(OpenSim::Storage *newMotion, bool primary, std::string filePath)
{

}

bool vsMotionsUtils::testMotionAssociationPossible(OpenSim::Model *model, OpenSim::Storage *storage)
{
    auto modelCS = model->getCoordinateSet();
    // Keep track of how many columns correspond to Coords or Markers
    int numberUsedColumnes = 0;
    for (int i = 0; i < modelCS.getSize(); ++i) {
        if(storage->getStateIndex(modelCS.get(i).getName()) != -1 ||
                storage->getStateIndex(modelCS.get(i).getStateVariableNames().get(0)) != -1){
            numberUsedColumnes ++;
            return true;
        }
    }

    OpenSim::Array<std::string> markersNames;
    model->getMarkerSet().getNames(markersNames);

    for(int i=0; i<markersNames.getSize(); i++){
        if ((storage->getStateIndex(markersNames.get(i)+"_tx")!=-1) ||
                (storage->getStateIndex(markersNames.get(i)+"_Tx")!=-1) ||
                (storage->getStateIndex(markersNames.get(i)+"_TX")!=-1))
            numberUsedColumnes++;
    }
    return (numberUsedColumnes >= 1);
}

void vsMotionsUtils::addMotion(OpenSim::Model *model, OpenSim::Storage *newMotion, OpenSim::Storage *parentMotion)
{

}
