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
#include <QSettings>

class vsNavigatorModel;
class vsNavigatorNode;
class vsModelNode;

/**
 * the class containing the global references and tools for the OpenSimQt application.
 * it contains the vsOpenSimTools::tools static instance that can be accessed from everywhere in the application
 * connected to the mainWindow to allow UI interactions such as logging .
 * contain an instace of the UI navigator model to allow it to be accesed without the need if importing the vsNavigatorModel or passing refernces.
 */

class vsOpenSimTools : public QObject
{
    Q_OBJECT
public:
    /**
     * specify the type and the color of a logged message
     */
    enum MessageType{
        Info, ///< usual information
        Success, ///< represent a successfull operation
        Warning, ///< represent an inconsistancy that can be skipped
        Error ///< represent an unsuccessfull operation output
    };
    Q_ENUM(MessageType)
    /**
     * the explecit constructor for openSimTools called once to instanciate the tools variable
     * @param parent the object that this instance live with and will be distroyed with the parent
     */
    vsOpenSimTools(QObject *parent = nullptr);
    static vsOpenSimTools *tools; ///< the only instance of vsOpenSimTools , accessed globaly
    QMap<OpenSim::Model*,SimTK::State*> modelStateMap; ///< storage linking each model with it state. @see vsNavigatorModel::loadOpenSimModel @see addToOpenModels
    QSettings* openSimSettings; ///< the settings for the whole application stored in config.ini
    //TODO documents the types of configurations in the config.ini file

    /**
     * log a message to the main interface and the log file.
     * it emit a signal that will be received by the vsMainWindow and log it
     * @param message the message RichText (Html,MD ...)
     * @param description the tag for the message
     * @param messageType   the degree of importance for the message
     * @param logToConsole wether to log to the UI defaut true
     * @see messageLogged
     * @see MessageType
     */
    void log(QString message,QString description="",MessageType messageType = MessageType::Info,bool logToConsole = true);
    void logPlainText(QString message);

    //settings
    /**
     * load the plugins form the plugin folder to opensim.
     * it checks the settings for plugins listed in the onEntry key
     * @see openSimSettings
     */
    void loadOnEntryPlugins();

    //openModels functions
    /**
     * addToOpenModels adds the model to the openModel variant.
     * it also add it to the openModels file, in order for it to be loaded when some actions like reload are triggered.
     * save the time of execution,path and name.
     * @param newModel the model to be added
     * @see updateOpenModelsFile
     * @see openModels
     */
    void addToOpenModels(OpenSim::Model *newModel);

    /**
     * remove the model from the openModels map.
     * usualy called when the model is closed
     * @param modelToRemove
     * @see addToOpenModels
     * @see updateOpenModelsFile
     */
    void removeFromOpenModels(OpenSim::Model *modelToRemove);
    /**
     * update the OpenSim.vs file with the current opened models.
     * called when a model is entered or removed.
     * @see addToOpenModelsFile
     * @see updateOpenModelsFile
     */
    void updateOpenModelsFile();
    /**
     * get the models to be reloaded paths.
     * @return a QStringList of models paths
     */
    QStringList getReloadModelsPaths();
    /**
     * save the current scene with it open models to a generated file.
     * @param savingFilePath the name of the file to save to (ususaly depend on time of save)
     * @see openModels
     */
    void saveScene(QString savingFilePath);

    //operations on the navigator
    /**
     * get the navigator node with its openSimObject equal to obj.
     * @param obj the object to find its node
     * @return a vsNavigatorNode of the given object or nullptr when nothing is found.
     * @see vsNavigatorModel::getNodeForObject
     */
    vsNavigatorNode* getObjectNode(OpenSim::Object *obj);

    vsNavigatorModel *getNavigatorModel() const; ///< getter for the Navigator model. @see m_navigatorModel
    void setNavigatorModel(vsNavigatorModel *navigatorModel);///< setter for the Navigator model . @see m_navigatorModel

    //other tools
    static QString getOSName(); ///< static functions used throughout the application to get the Operating system running the application.
    static QString getPluginExtentionForOS(); ///< get the plugin extension for libraries depending on the OS. @see getOSName

public slots:
    void onCurrentModelUpdated(); ///< slot to handle the changes when the current model has changed. @see currentModelUpdated

signals:
    void messageLogged(QString message); ///< signal to notify the MainWindow to log a Rich Text. @see log
    void messageLoggedPlain(QString message);///< signal to notify the MainWindow to log a Plain Text. @see logPlainText
    void currentModelUpdated();///< signal emited when the active model has changed

private:
    QFile *logFile; ///< the file to store the application log durring its execution.
    QTextStream *logStream; ///< the log stream that link the messages to their output destination (file or console)
    QMap<MessageType,QString> messageColors; ///< map to get the message type color
    vsNavigatorModel *m_navigatorModel; ///< the main entry to the Navigator througout the application, to updates the nodes inside it.

    //open_models variables
    /// the variant map storring the open models informations. information like the time of opening, path, name
    /// @see addToOpenModels @see removeFromOpenModels
    QVariantMap openModels;


};

#endif // VSOPENSIMTOOLS_H
