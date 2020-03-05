#ifndef vsOneMarkerNode_H
#define vsOneMarkerNode_H

#include "vsNavigatorNode.h"



class vsOneMarkerNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsOneMarkerNode(OpenSim::Marker *marker,vsNavigatorNode *parentNode,QObject *parent);
};

#endif // vsOneMarkerNode_H
