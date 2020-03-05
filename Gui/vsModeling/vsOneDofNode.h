#ifndef vsOneDofNode_H
#define vsOneDofNode_H

#include "vsNavigatorNode.h"



class vsOneDofNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsOneDofNode(OpenSim::TransformAxis *tAxis,vsNavigatorNode *parentNode,QObject *parent);
};

#endif // vsOneDofNode_H
