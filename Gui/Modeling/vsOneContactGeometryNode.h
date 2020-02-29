#ifndef vsOneContactGeometryNode_H
#define vsOneContactGeometryNode_H

#include "vsNavigatorNode.h"



class vsOneContactGeometryNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsOneContactGeometryNode(OpenSim::ContactGeometry *contactGeometry,vsNavigatorNode *parentNode,QObject *parent);
};

#endif // vsOneContactGeometryNode_H
