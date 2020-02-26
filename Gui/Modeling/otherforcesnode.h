#ifndef OTHERFORCESNODE_H
#define OTHERFORCESNODE_H

#include "navigatornode.h"



class OtherForcesNode : public NavigatorNode
{
    Q_OBJECT
public:
    OtherForcesNode(OpenSim::ForceSet *forceSet,NavigatorNode *parentNode, QObject *parent);
};

#endif // OTHERFORCESNODE_H
