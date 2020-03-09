#ifndef vsAllForcesNode_H
#define vsAllForcesNode_H

#include "vsNavigatorNode.h"



class vsAllForcesNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsAllForcesNode(OpenSim::Model *model,vsNavigatorNode *parentNode,QObject *parent);

};

#endif // vsAllForcesNode_H
