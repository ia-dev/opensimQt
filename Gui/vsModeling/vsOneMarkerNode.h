#ifndef vsOneMarkerNode_H
#define vsOneMarkerNode_H

#include "vsNavigatorNode.h"



class vsOneMarkerNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsOneMarkerNode(OpenSim::Marker *marker,vsNavigatorNode *parentNode,QObject *parent);

    // vsNavigatorNode interface
public:
    virtual void setupNodeActions(QMenu *rootMenu) override;
};

#endif // vsOneMarkerNode_H
