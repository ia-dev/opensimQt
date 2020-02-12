#ifndef CONSTRAINTSNODE_H
#define CONSTRAINTSNODE_H

#include "navigatornode.h"



class ConstraintsNode : public NavigatorNode
{
    Q_OBJECT
public:
    ConstraintsNode(OpenSim::ConstraintSet *constraintSet,NavigatorNode *parentNode,QObject *parent);
};

#endif // CONSTRAINTSNODE_H
