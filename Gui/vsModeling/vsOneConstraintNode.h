#ifndef vsOneConstraintNode_H
#define vsOneConstraintNode_H

#include "vsNavigatorNode.h"



class vsOneConstraintNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsOneConstraintNode(OpenSim::Constraint *constraint,vsNavigatorNode *parentNode,QObject *parent);
};

#endif // vsOneConstraintNode_H
