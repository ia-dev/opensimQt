#ifndef vsMusclesNode_H
#define vsMusclesNode_H

#include "vsNavigatorNode.h"



class vsMusclesNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsMusclesNode(OpenSim::ForceSet *forceSet,OpenSim::Set<OpenSim::Muscle> *muscleSet,vsNavigatorNode *parentNode,QObject *parent);
};

#endif // vsMusclesNode_H
