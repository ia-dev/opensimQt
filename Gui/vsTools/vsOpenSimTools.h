/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#ifndef VSOPENSIMTOOLS_H
#define VSOPENSIMTOOLS_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QWidget>
#include <OpenSim.h>
#include <QVariantMap>

class vsOpenSimTools : public QObject
{
    Q_OBJECT
public:
    enum MessageType{
        Info,Success,Warning,Error
    };
    Q_ENUM(MessageType)
    vsOpenSimTools(QObject *parent = nullptr);
    static vsOpenSimTools *tools;
    void log(QString message,QString description="",MessageType messageType = MessageType::Info,bool logToConsole = true);

    //openModels functions
    void addToOpenModels(OpenSim::Model *newModel);
    void removeFromOpenModels(OpenSim::Model *modelToRemove);
    void updateOpenModelsFile();
    QStringList getReloadModelsPaths();
signals:
    void messageLogged(QString message);

private:
    QFile *logFile;
    QTextStream *logStream;
    QMap<MessageType,QString> messageColors;

    //open_models variables
    QVariantMap openModels;


};

#endif // VSOPENSIMTOOLS_H
