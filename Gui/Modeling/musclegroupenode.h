#ifndef MUSCLEGROUPENODE_H
#define MUSCLEGROUPENODE_H

#include "navigatornode.h"



class MuscleGroupeNode : public NavigatorNode
{
    Q_OBJECT
public:
    MuscleGroupeNode(OpenSim::ObjectGroup *muscleGroup,OpenSim::Set<OpenSim::Muscle> muscles,NavigatorNode *parentNode,QObject *parent);
};

#endif // MUSCLEGROUPENODE_H
