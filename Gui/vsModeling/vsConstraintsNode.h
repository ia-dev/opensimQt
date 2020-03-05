#ifndef vsConstraintsNode_H
#define vsConstraintsNode_H

#include "vsNavigatorNode.h"



class vsConstraintsNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsConstraintsNode(OpenSim::ConstraintSet *constraintSet,vsNavigatorNode *parentNode,QObject *parent);
};

#endif // vsConstraintsNode_H
