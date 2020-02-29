#ifndef vsNavigatorNode_H
#define vsNavigatorNode_H

#include <QObject>
#include <OpenSim/OpenSim.h>
#include <Visualizer/vsVisualizerVTK.h>

class vsNavigatorNode : public QObject
{
    Q_OBJECT
public:
    explicit vsNavigatorNode(OpenSim::Object *_openSimObj,QString _displayName,vsNavigatorNode *_parentNode,QObject *parent = nullptr);


public:
    OpenSim::Object *openSimObject;
    QString displayName;
    vsNavigatorNode* parentNode;
    QList<vsNavigatorNode*> childNodes;
    QString iconPath;
    static vsVisualizerVTK *visualizerVTK;
signals:

};

#endif // vsNavigatorNode_H
