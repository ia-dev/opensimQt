#ifndef vsMuscleGroupeNode_H
#define vsMuscleGroupeNode_H

#include "vsNavigatorNode.h"



class vsMuscleGroupeNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsMuscleGroupeNode(OpenSim::ObjectGroup *muscleGroup,OpenSim::Set<OpenSim::Muscle> muscles,vsNavigatorNode *parentNode,QObject *parent);
};

#endif // vsMuscleGroupeNode_H
