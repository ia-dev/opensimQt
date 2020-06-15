/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#include "vsOpenSimTools.h"

#include <QDebug>
#include <QJsonDocument>
#include <QTextEdit>
#include <QTime>

#include "vsModeling/vsNavigatorModel.h"

vsOpenSimTools* vsOpenSimTools::tools = new vsOpenSimTools();

vsOpenSimTools::vsOpenSimTools(QObject *parent):QObject(parent),
    logFile(new QFile("OpenSimQtLog_.txt")),m_navigatorModel(nullptr)
{
    logFile->open(QFile::WriteOnly|QFile::Text);
    if(logFile->isOpen()) qDebug() << "it is open , the file";
    logStream = new QTextStream(logFile);
    *logStream << "Launching OpenSimQt ..." << endl;
    logStream->flush();

    messageColors.insert(MessageType::Info,"#888888");
    messageColors.insert(MessageType::Success,"#55B83B");
    messageColors.insert(MessageType::Warning,"#ffff00");
    messageColors.insert(MessageType::Error,"#ff0000");
}

void vsOpenSimTools::log(QString message, QString description,MessageType messageType,bool logToConsole)
{
    QString typeText = QVariant::fromValue(messageType).toString();
    QString messageLine = (description==""?"":(description+"::")) + message;
    *logStream << "["<<QDateTime::currentDateTime().toString()<< "]"<< messageLine << endl ;
    logStream->flush();
    QString consoleLine ="<div><span style='color:" + messageColors.value(messageType,"#000000") +"'>#" + typeText +"::</span> "
                          +  messageLine + "</div>";
    if(logToConsole)
        emit messageLogged(consoleLine);


}

void vsOpenSimTools::addToOpenModels(OpenSim::Model *newModel)
{
    QVariantMap modelVariant;
    modelVariant.insert("name",QString::fromStdString(newModel->getName()));
    modelVariant.insert("time",QDateTime::currentDateTime().toString());
    modelVariant.insert("path",QString::fromStdString(newModel->getInputFileName()));
    openModels.insert(QString::fromStdString(newModel->getName()),modelVariant);
    updateOpenModelsFile();
    log("Model added","Navigator",Success,true);
}

void vsOpenSimTools::removeFromOpenModels(OpenSim::Model *modelToRemove)
{
    openModels.remove(QString::fromStdString(modelToRemove->getName()));
    updateOpenModelsFile();
}

void vsOpenSimTools::updateOpenModelsFile()
{
    QJsonDocument openModelsDocument = QJsonDocument::fromVariant(this->openModels);
    QFile openModelsFile("OpenSim.vs");
    openModelsFile.open(QFile::WriteOnly|QFile::Text);
    openModelsFile.write(openModelsDocument.toJson());
    openModelsFile.close();
}

QStringList vsOpenSimTools::getReloadModelsPaths()
{
    QStringList returnList;
    foreach (QVariant modelVariant, openModels.values()) {
        returnList << modelVariant.toMap().value("path").toString();
    }
    return returnList;
}

void vsOpenSimTools::saveScene(QString savingFilePath)
{
    QJsonDocument openModelsDocument = QJsonDocument::fromVariant(this->openModels);
    QFile openModelsFile(savingFilePath);
    openModelsFile.open(QIODevice::WriteOnly|QIODevice::Text);
    openModelsFile.write(openModelsDocument.toJson());
    openModelsFile.close();
}

vsNavigatorNode *vsOpenSimTools::getObjectNode(OpenSim::Object *obj)
{
    return m_navigatorModel->getNodeForObject(obj);
}

vsNavigatorModel *vsOpenSimTools::getNavigatorModel() const
{
    return m_navigatorModel;
}

void vsOpenSimTools::setNavigatorModel(vsNavigatorModel *navigatorModel)
{
    m_navigatorModel = navigatorModel;
}




