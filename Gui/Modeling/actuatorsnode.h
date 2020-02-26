#ifndef ACTUATORSNODE_H
#define ACTUATORSNODE_H

#include "navigatornode.h"



class ActuatorsNode : public NavigatorNode
{
    Q_OBJECT
public:
    ActuatorsNode(OpenSim::ForceSet *forceSet,NavigatorNode *parentNode,QObject *parent);
};

#endif // ACTUATORSNODE_H
