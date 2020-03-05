#ifndef vsOneProbNode_H
#define vsOneProbNode_H

#include "vsNavigatorNode.h"



class vsOneProbNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsOneProbNode(OpenSim::Probe *probe,vsNavigatorNode *parentNode,QObject *parent);
};

#endif // vsOneProbNode_H
