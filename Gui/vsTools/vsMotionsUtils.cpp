/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                          *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#include "vsMotionsUtils.h"

#include <QApplication>
#include <QBitArray>
#include <QDebug>
#include <QFileDialog>
#include "vsOpenSimTools.h"
#include <vsModeling/vsNavigatorModel.h>
#include <vsModeling/vsOneMotionNode.h>

vsMotionsUtils* vsMotionsUtils::instance = nullptr;

vsMotionsUtils::vsMotionsUtils(QObject *parent) : QObject(parent)
{
    //handle the changes in this class too
    connect(this,&vsMotionsUtils::notifyObservers,this,&vsMotionsUtils::update);
}

vsMotionsUtils *vsMotionsUtils::getInstance()
{
    if(!instance) instance = new vsMotionsUtils();
    return instance;
}

void vsMotionsUtils::openLoadMotionDialog(OpenSim::Model *model)
{
    //model->realizePosition(model->getWorkingState());
    try {
        QString motionFile = QFileDialog::getOpenFileName(nullptr,"Load Motion File To Current Model","","Motions (*.mot *.sto)");
        if(motionFile == "") throw  QString("error");

        OpenSim::Storage *storage = new OpenSim::Storage(motionFile.toStdString());

        activeModel = model;

        loadMotionStorage(storage,true,motionFile.toStdString());
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
    if(!activeModel) return;
    if(true){//testMotionAssociationPossible(activeModel,newMotion)){
        addMotion(activeModel,newMotion,nullptr);
        vsOpenSimTools::tools->log("Motion : "+QString::fromStdString(newMotion->getName())+
                                   " Loaded to model : "+QString::fromStdString(activeModel->getName()),
                                   "vsMotionUtils",vsOpenSimTools::Success);
    }
    else{
        vsOpenSimTools::tools->log("Could not apply motion to active model","vsMotionUtils",vsOpenSimTools::Error);
    }
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
    //test for the Model For Experement Data
    bool convertAngels = newMotion->isInDegrees();
    if(convertAngels) model->getSimbodyEngine().convertDegreesToRadians(*newMotion);

    auto modelMotions = mapModelsToMotions.value(model,nullptr);
    if(!modelMotions){
        modelMotions = new QList<OpenSim::Storage*>();
        mapModelsToMotions.insert(model,modelMotions);
    }

    modelMotions->append(newMotion);
    //TODO change 4 to bitnumbers
    //mapMotionToBitArray.insert(newMotion,new QBitArray(4));
    MotionEventObject evntObj(model,newMotion,parentMotion?MotionOperation::Assoc:MotionOperation::Open);
    emit notifyObservers(evntObj);

    if(parentMotion == nullptr) setCurrentMotion(model,newMotion);
}

void vsMotionsUtils::setCurrentMotion(OpenSim::Model *model, OpenSim::Storage *motion)
{
    //TODO add multiple motions support
    activeModel = model;
    currentMotion= new QPair<OpenSim::Model*,OpenSim::Storage*>(model,motion);
    applyTimeToModel(model,motion,motion->getFirstTime());
    MotionEventObject evntObj(model,motion,MotionOperation::CurrentMotionsChanged);
    emit notifyObservers(evntObj);
    emit currentMotionChanged();
}

void vsMotionsUtils::applyTimeToModel(OpenSim::Model *model, OpenSim::Storage *motion,double time)
{
    int startFrame = 0;
    int endFrame = 0;
    //TODO Use the timer step
    motion->findFrameRange(time,time+30,startFrame,endFrame);
    auto stateData = motion->getStateVector(startFrame)->getData();
    try {
        //trying to use names to access coordinades
        //qDebug("printing coordinate names in OpenSim");
        for (int i= 0;i < model->getNumCoordinates(); i++) {
            auto coordinateName = model->updCoordinateSet().get(i).getName();
            int stateIndex = motion->getStateIndex(coordinateName);
            auto coordValue = stateData.get(stateIndex);
            model->updCoordinateSet().get(i).setValue(model->updWorkingState(),coordValue);
            //qDebug() << "coordinate name : " << QString::fromStdString(coordinateName) << "found with index: " << stateIndex;
        }
    } catch (...) {
        //vsOpenSimTools::tools->log("coordinates names are mismatch, switching to indices","vsMotionUtils",vsOpenSimTools::Warning);
        try {
            int numbCoordinates = model->getNumCoordinates();
            for (int i = 0; i < numbCoordinates; ++i) {
                auto coordValue = stateData.get(i);
                model->updCoordinateSet().get(i).setValue(model->updWorkingState(),coordValue);
                //qDebug() << "corrdinate name in model : " << QString::fromStdString(model->updCoordinateSet().get(i).getName());
            }
        } catch (...) {
            //vsOpenSimTools::tools->log("both names and indicies solutions didnt work","vsMotionsUtils",vsOpenSimTools::Error);
        }
    }


    //qDebug() << endl << endl << "printing names in the motion file";
//    for (int i = 0; i < motion->getColumnLabels().getSize(); ++i) {
//        qDebug() << "corrdinate name in motion : " << QString::fromStdString(motion->getColumnLabels().get(i));
//    }
    //get the coordinates using names
    //model->upd_ComponentSet().get

    //SimTK::Vector stateData(motion->getColumnLabels().getSize()-1);
    //double ** stateData;
    //the minus one is to exclude the time column
    //motion->getDataAtTime(time,motion->getColumnLabels().getSize()-1,stateData);
    //auto stateVector = motion->getStateVector(time);
    //model->updWorkingState().setTime(time);

    //int numberOfCoordinates = model->getcoo

    //model->setStateVariableValues(model->updWorkingState(),stateData);
    //model->realizeTime(model->updWorkingState());
    model->realizeDynamics(model->updWorkingState());
    //model->realizeVelocity(model->updWorkingState());
    //qDebug() << "the state of the model is updated" << motion->getColumnLabels().getSize();
    //TODO update the decorations instead of removing
    //model->realizeDynamics(model->updWorkingState());
    vsNavigatorNode::visualizerVTK->updating = true;
    //vsNavigatorNode::visualizerVTK->removeModelActors(model);
    //vsNavigatorNode::visualizerVTK->addOpenSimModel(model);
    vsNavigatorNode::visualizerVTK->updateModelDecorations(model);
}

void vsMotionsUtils::applyFrameToModel(OpenSim::Model *model, OpenSim::Storage *motion, int framNumber)
{
    auto stateData = motion->getStateVector(framNumber)->getData();
    int numbCoordinates = model->getNumCoordinates();
    for (int i = 0; i < numbCoordinates; ++i) {
        auto coordValue = stateData.get(i);
        model->updCoordinateSet().get(i).setValue(model->updWorkingState(),coordValue);
    }
    model->realizePosition(model->updWorkingState());
    vsNavigatorNode::visualizerVTK->updating = true;
    vsNavigatorNode::visualizerVTK->updateModelDecorations(model);
}

void vsMotionsUtils::applySimulationToCurrentModel(double endTime)
{
    auto model = vsOpenSimTools::tools->getNavigatorModel()->getActiveModel();
    applySimulationToModel(model,endTime);
}

void vsMotionsUtils::applySimulationToCurrentModelM(double endTime, double accuracy, double stepSize, OpenSim::Manager::IntegratorMethod integrator)
{
    auto model = vsOpenSimTools::tools->getNavigatorModel()->getActiveModel();
    applySimulaitonToModelUsingManager(model,endTime,accuracy,stepSize,integrator);
}

void vsMotionsUtils::applySimulationToModel(OpenSim::Model *model, double endTime)
{
    qDebug() << "applying simulation to this model " << QString::fromStdString(model->getName());
    OpenSim::simulate(*model,model->updWorkingState(),endTime,true);
    qDebug() << "simulation ended playing results for: " << QString::fromStdString(model->getName());
    //moving the motion file to a motions folder

    //load it from the the storage file
    try {
        QString motionFile = QApplication::applicationDirPath()+"/"+QString::fromStdString(model->getName())+"_states.sto";
        if(motionFile == "") throw  QString("error");

        OpenSim::Storage *storage = new OpenSim::Storage(motionFile.toStdString());

        activeModel = model;

        loadMotionStorage(storage,true,motionFile.toStdString());
        vsOpenSimTools::tools->log("motion file selected :"+motionFile,"vsModelNode");
    }
    catch (QString s) {
        vsOpenSimTools::tools->log("no file was selected :","vsModelNode",vsOpenSimTools::Error);
    }
}

void vsMotionsUtils::applySimulaitonToModelUsingManager(OpenSim::Model *model, double endTime, double accuracy, double stepSize, OpenSim::Manager::IntegratorMethod integrator)
{
    OpenSim::Storage *outputStorage = new OpenSim::Storage();

    OpenSim::ForceReporter* fReporter    = new OpenSim::ForceReporter(model);
    OpenSim::StatesReporter* sReporter   = new OpenSim::StatesReporter(model);
    OpenSim::BodyKinematics* bKinematics = new OpenSim::BodyKinematics(model, true);

    OpenSim::ConsoleReporter *cReporter = new OpenSim::ConsoleReporter();

    OpenSim::Array<std::string> bodiesToRecord ;
    bKinematics->setModel(*model);
    bKinematics->setBodiesToRecord(bodiesToRecord);
    model->addAnalysis(fReporter);
    model->addAnalysis(sReporter);
    model->addAnalysis(bKinematics);

    //model->addComponent(cReporter);

    OpenSim::Manager manager(*model);
    manager.setWriteToStorage(true);
    manager.setIntegratorMethod(integrator);
    manager.setIntegratorAccuracy(accuracy);
    manager.setIntegratorMaximumStepSize(stepSize);

    std::ostringstream managerOutput;

    model->updWorkingState().setTime(0.0);
    model->printDetailedInfo(model->updWorkingState(),managerOutput);
    managerOutput << std::endl;

    manager.initialize(model->updWorkingState());
    managerOutput << endl;
    managerOutput << "integrating from" << QString::number(0).toStdString() << " to " << QString::number(endTime).toStdString();
    manager.integrate(endTime);
    QDir simulationsDir(QApplication::applicationDirPath());
    if(!simulationsDir.exists(simulationsDir.path()+"Simulations"))simulationsDir.mkdir("Simulations");
    simulationsDir.cd("Simulations");
    int fR = fReporter->printResults(model->getName(),  simulationsDir.path().toStdString(),stepSize);
    int sR = sReporter->printResults(model->getName(),  simulationsDir.path().toStdString(),stepSize);
    int kR = bKinematics->printResults(model->getName(),simulationsDir.path().toStdString(),stepSize);

    std::string reporterOutput;

    vsOpenSimTools::tools->log(QString::fromStdString(managerOutput.str()),"vsMotionUtils");
    //vsOpenSimTools::tools->log(QString::fromStdString(),"vsMotionUtils");
    activeModel = model;
    loadMotionStorage(&manager.getStateStorage(),true,"");

}

void vsMotionsUtils::update(MotionEventObject eventObj)
{
    //TODO support other events
    qDebug()<< "motion is being updated";

    //motion event
    switch (eventObj.m_motionOperation) {
    case MotionOperation::Open:{
        vsNavigatorNode *modelNode = vsOpenSimTools::tools->getObjectNode(eventObj.m_model);
        vsNavigatorNode *motionsNode = modelNode->findChildNode("Motions");
        if(motionsNode == nullptr){
            motionsNode = new vsNavigatorNode(nullptr,"Motions",modelNode,modelNode);
            motionsNode->iconPath = ":/Data/Images/Nodes/motionsNode.png";
        }

        //TODO add suppoet for experemental data model
        vsOneMotionNode *newMotionNode = new vsOneMotionNode(eventObj.m_storage,eventObj.m_model,motionsNode,motionsNode);
        qDebug()<< "Motion Name : " << QString::fromStdString(eventObj.m_storage->getName());
        emit vsOpenSimTools::tools->getNavigatorModel()->layoutChanged();
    }break;

    default:
        break;
    }

}
