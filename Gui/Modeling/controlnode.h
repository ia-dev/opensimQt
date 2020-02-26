#ifndef CONTROLNODE_H
#define CONTROLNODE_H

#include "navigatornode.h"



class ControlNode : public NavigatorNode
{
    Q_OBJECT
public:
    ControlNode(OpenSim::Controller *control ,NavigatorNode *parentNode,QObject *parent);
};

#endif // CONTROLNODE_H
