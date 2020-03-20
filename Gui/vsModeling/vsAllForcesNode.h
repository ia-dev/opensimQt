#ifndef vsAllForcesNode_H
#define vsAllForcesNode_H

#include "vsNavigatorNode.h"
#include "vsObjectSetNode.h"



class vsAllForcesNode : public vsObjectSetNode
{
    Q_OBJECT
public:
    vsAllForcesNode(OpenSim::Model *model,vsNavigatorNode *parentNode,QObject *parent);

};

#endif // vsAllForcesNode_H
