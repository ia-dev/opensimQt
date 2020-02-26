#ifndef ONEOTHERFORCENODE_H
#define ONEOTHERFORCENODE_H

#include "navigatornode.h"



class OneOtherForceNode : public NavigatorNode
{
    Q_OBJECT
public:
    OneOtherForceNode(OpenSim::Force *otherForce,NavigatorNode *parentNode, QObject *parent);
};

#endif // ONEOTHERFORCENODE_H
