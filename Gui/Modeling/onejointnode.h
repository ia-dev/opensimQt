#ifndef ONEJOINTNODE_H
#define ONEJOINTNODE_H

#include "navigatornode.h"



class OneJointNode : public NavigatorNode
{
    Q_OBJECT
public:
    OneJointNode(OpenSim::Joint *joint,NavigatorNode *parentNode,QObject *parent);
};

#endif // ONEJOINTNODE_H
