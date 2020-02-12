#ifndef ONEMUSCLENODE_H
#define ONEMUSCLENODE_H

#include "navigatornode.h"



class OneMuscleNode : public NavigatorNode
{
    Q_OBJECT
public:
    OneMuscleNode(OpenSim::Muscle *muscle,NavigatorNode *parentNode,QObject *parent);
};

#endif // ONEMUSCLENODE_H
