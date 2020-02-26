#ifndef JOINTSNODE_H
#define JOINTSNODE_H

#include "navigatornode.h"



class JointsNode : public NavigatorNode
{
    Q_OBJECT
public:
    JointsNode(OpenSim::JointSet *jointSet,NavigatorNode* parentNode,QObject *parent);
};

#endif // JOINTSNODE_H
