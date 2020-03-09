#ifndef vsMarkersNode_H
#define vsMarkersNode_H

#include "vsNavigatorNode.h"



class vsMarkersNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsMarkersNode(OpenSim::MarkerSet *markerSet,vsNavigatorNode *parentNode,QObject *parent);
};

#endif // vsMarkersNode_H
