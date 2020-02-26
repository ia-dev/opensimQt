#ifndef MARKERSNODE_H
#define MARKERSNODE_H

#include "navigatornode.h"



class MarkersNode : public NavigatorNode
{
    Q_OBJECT
public:
    MarkersNode(OpenSim::MarkerSet *markerSet,NavigatorNode *parentNode,QObject *parent);
};

#endif // MARKERSNODE_H
