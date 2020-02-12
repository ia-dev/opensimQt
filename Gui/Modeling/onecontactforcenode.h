#ifndef ONECONTACTFORCENODE_H
#define ONECONTACTFORCENODE_H

#include "navigatornode.h"



class OneContactForceNode : public NavigatorNode
{
    Q_OBJECT
public:
    OneContactForceNode(OpenSim::Force *contactForce,NavigatorNode *parentNode,QObject *parent);
};

#endif // ONECONTACTFORCENODE_H
