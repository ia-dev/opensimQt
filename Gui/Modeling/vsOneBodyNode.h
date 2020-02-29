#ifndef vsOneBodyNode_H
#define vsOneBodyNode_H

#include "vsOneFrameNode.h"



class vsOneBodyNode : public vsOneFrameNode
{
    Q_OBJECT
public:
    vsOneBodyNode(OpenSim::Body *body,vsNavigatorNode *parentNode,QObject *parent);
};

#endif // vsOneBodyNode_H
