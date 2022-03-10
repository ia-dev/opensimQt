#ifndef vsNavigatorNode_H
#define vsNavigatorNode_H


#include <QMenu>
#include <QObject>
#include <OpenSim/OpenSim.h>
#include <vsVisualizer/vsVisualizerVTK.h>

class vsNavigatorModel;
class vsPropertyModel;

class vsNavigatorNode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(vsNavigatorModel* connectedModel READ connectedModel WRITE setConnectedModel NOTIFY connectedModelChanged)
    vsNavigatorModel* m_connectedModel;

public:
    explicit vsNavigatorNode(OpenSim::Object *_openSimObj,QString _displayName,vsNavigatorNode *_parentNode,QObject *parent = nullptr);

    ~vsNavigatorNode();

    virtual void setupNodeActions(QMenu *rootMenu);
    virtual QVariantMap getNodeProperties();
    virtual void setupPropertiesModel(vsPropertyModel *model);

    void selectVisualizerActors();
    void disableActionsForSets();
    vsNavigatorModel* connectedModel() const;
    void removeNode();

    vsNavigatorNode* getNodeForObj(OpenSim::Object *object);
    vsNavigatorNode *findChildNode(QString childName);
    OpenSim::Object *openSimObject;
    QString displayName;
    vsNavigatorNode* parentNode;
    QList<vsNavigatorNode*> childNodes;
    QString iconPath;
    QString FilePath;
    QMenu *displayMenu;
    bool editColorAndOpacity = true;
    static vsVisualizerVTK *visualizerVTK;

public slots:
    void setConnectedModel(vsNavigatorModel* connectedModel);

    void onShowNodeTriggerd();
    void onHideNodeTriggerd();

signals:

    void connectedModelChanged(vsNavigatorModel* connectedModel);
};

#endif // vsNavigatorNode_H
