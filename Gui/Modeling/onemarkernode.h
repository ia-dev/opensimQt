#ifndef ONEMARKERNODE_H
#define ONEMARKERNODE_H

#include "navigatornode.h"



class OneMarkerNode : public NavigatorNode
{
    Q_OBJECT
public:
    OneMarkerNode(OpenSim::Marker *marker,NavigatorNode *parentNode,QObject *parent);
};

#endif // ONEMARKERNODE_H
