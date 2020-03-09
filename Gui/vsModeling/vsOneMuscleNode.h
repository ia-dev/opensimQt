#ifndef vsOneMuscleNode_H
#define vsOneMuscleNode_H

#include "vsNavigatorNode.h"



class vsOneMuscleNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsOneMuscleNode(OpenSim::Muscle *muscle,vsNavigatorNode *parentNode,QObject *parent);
};

#endif // vsOneMuscleNode_H
