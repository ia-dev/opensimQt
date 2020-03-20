#ifndef vsMarkersNode_H
#define vsMarkersNode_H

#include "vsNavigatorNode.h"



class vsMarkersNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsMarkersNode(OpenSim::MarkerSet *markerSet,vsNavigatorNode *parentNode,QObject *parent);

    // vsNavigatorNode interface
public:
    virtual void setupNodeActions(QMenu *rootMenu) override;
};

#endif // vsMarkersNode_H
