#ifndef vsOneMuscleNode_H
#define vsOneMuscleNode_H

#include "vsNavigatorNode.h"



class vsOneMuscleNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsOneMuscleNode(OpenSim::Muscle *muscle,vsNavigatorNode *parentNode,QObject *parent);

    // vsNavigatorNode interface
public:
    virtual void setupNodeActions(QMenu *rootMenu) override;
};

#endif // vsOneMuscleNode_H
