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

class vsNavigatorModel;
class vsNavigatorNode;
class vsModelNode;

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
    QMap<OpenSim::Model*,SimTK::State*> modelStateMap;
    void log(QString message,QString description="",MessageType messageType = MessageType::Info,bool logToConsole = true);
    void logPlainText(QString message);

    //openModels functions
    void addToOpenModels(OpenSim::Model *newModel);
    void removeFromOpenModels(OpenSim::Model *modelToRemove);
    void updateOpenModelsFile();
    QStringList getReloadModelsPaths();
    void saveScene(QString savingFilePath);

    //operations on the navigator
    vsNavigatorNode* getObjectNode(OpenSim::Object *obj);

    vsNavigatorModel *getNavigatorModel() const;
    void setNavigatorModel(vsNavigatorModel *navigatorModel);

    //other tools
    static QString getOSName();
    static QString getPluginExtentionForOS();

public slots:
    void onCurrentModelUpdated();

signals:
    void messageLogged(QString message);
    void messageLoggedPlain(QString message);
    void currentModelUpdated();

private:
    QFile *logFile;
    QTextStream *logStream;
    QMap<MessageType,QString> messageColors;
    vsNavigatorModel *m_navigatorModel;

    //open_models variables
    QVariantMap openModels;


};

#endif // VSOPENSIMTOOLS_H
