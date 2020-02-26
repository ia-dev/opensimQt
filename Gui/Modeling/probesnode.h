#ifndef PROBESNODE_H
#define PROBESNODE_H

#include "navigatornode.h"



class ProbesNode : public NavigatorNode
{
    Q_OBJECT
public:
    ProbesNode(OpenSim::ProbeSet *probSet,NavigatorNode *parentNode,QObject *parent);
};

#endif // PROBESNODE_H
