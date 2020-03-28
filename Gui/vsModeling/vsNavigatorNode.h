#ifndef vsNavigatorNode_H
#define vsNavigatorNode_H

#include <QMenu>
#include <QObject>
#include <OpenSim/OpenSim.h>
#include <vsVisualizer/vsVisualizerVTK.h>

class vsNavigatorModel;

class vsNavigatorNode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(vsNavigatorModel* connectedModel READ connectedModel WRITE setConnectedModel NOTIFY connectedModelChanged)
    vsNavigatorModel* m_connectedModel;

public:
    explicit vsNavigatorNode(OpenSim::Object *_openSimObj,QString _displayName,vsNavigatorNode *_parentNode,QObject *parent = nullptr);

    ~vsNavigatorNode();

    virtual void setupNodeActions(QMenu *rootMenu);

    void disableActionsForSets();
    vsNavigatorModel* connectedModel() const;
    void removeNode();
public:
    OpenSim::Object *openSimObject;
    QString displayName;
    vsNavigatorNode* parentNode;
    QList<vsNavigatorNode*> childNodes;
    QString iconPath;
    QMenu *displayMenu;
    bool editColorAndOpacity = true;
    static vsVisualizerVTK *visualizerVTK;

public slots:
    void setConnectedModel(vsNavigatorModel* connectedModel);

signals:

    void connectedModelChanged(vsNavigatorModel* connectedModel);
};

#endif // vsNavigatorNode_H
