#ifndef vsOneJointNode_H
#define vsOneJointNode_H

#include "vsNavigatorNode.h"



class vsOneJointNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsOneJointNode(OpenSim::Joint *joint,vsNavigatorNode *parentNode,QObject *parent);

    // vsNavigatorNode interface
public:
    virtual void setupNodeActions(QMenu *rootMenu) override;
};

#endif // vsOneJointNode_H
