#ifndef CONTACTFORCESNODE_H
#define CONTACTFORCESNODE_H

#include "navigatornode.h"



class ContactForcesNode : public NavigatorNode
{
    Q_OBJECT
public:
    ContactForcesNode(OpenSim::ForceSet *forceSet,NavigatorNode *parentNode,QObject *parent);
};

#endif // CONTACTFORCESNODE_H
