#ifndef MUSCLESNODE_H
#define MUSCLESNODE_H

#include "navigatornode.h"



class MusclesNode : public NavigatorNode
{
    Q_OBJECT
public:
    MusclesNode(OpenSim::ForceSet *forceSet,OpenSim::Set<OpenSim::Muscle> *muscleSet,NavigatorNode *parentNode,QObject *parent);
};

#endif // MUSCLESNODE_H
