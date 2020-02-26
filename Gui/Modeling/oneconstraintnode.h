#ifndef ONECONSTRAINTNODE_H
#define ONECONSTRAINTNODE_H

#include "navigatornode.h"



class OneConstraintNode : public NavigatorNode
{
    Q_OBJECT
public:
    OneConstraintNode(OpenSim::Constraint *constraint,NavigatorNode *parentNode,QObject *parent);
};

#endif // ONECONSTRAINTNODE_H
