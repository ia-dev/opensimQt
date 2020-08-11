#ifndef vsBodiesNode_H
#define vsBodiesNode_H

#include "vsNavigatorNode.h"



class vsBodiesNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsBodiesNode(OpenSim::BodySet *bodySet,vsNavigatorNode *parentNode,QObject *parent);
};

#endif // vsBodiesNode_H
