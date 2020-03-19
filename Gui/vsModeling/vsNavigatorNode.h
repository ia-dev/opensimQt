#ifndef vsNavigatorNode_H
#define vsNavigatorNode_H

#include <QMenu>
#include <QObject>
#include <OpenSim/OpenSim.h>
#include <vsVisualizer/vsVisualizerVTK.h>

class vsNavigatorNode : public QObject
{
    Q_OBJECT
public:
    explicit vsNavigatorNode(OpenSim::Object *_openSimObj,QString _displayName,vsNavigatorNode *_parentNode,QObject *parent = nullptr);

    ~vsNavigatorNode();

    virtual void setupNodeActions(QMenu *rootMenu);
public:
    OpenSim::Object *openSimObject;
    QString displayName;
    vsNavigatorNode* parentNode;
    QList<vsNavigatorNode*> childNodes;
    QString iconPath;
    QMenu *displayMenu;
    static vsVisualizerVTK *visualizerVTK;
signals:

};

#endif // vsNavigatorNode_H
