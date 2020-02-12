#ifndef CONTROLERSNODE_H
#define CONTROLERSNODE_H

#include "navigatornode.h"



class ControlersNode : public NavigatorNode
{
    Q_OBJECT
public:
    ControlersNode(OpenSim::ControllerSet *controlSet ,NavigatorNode *parentNode,QObject *parent);
};

#endif // CONTROLERSNODE_H
