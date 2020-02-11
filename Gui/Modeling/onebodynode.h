#ifndef ONEBODYNODE_H
#define ONEBODYNODE_H

#include "oneframenode.h"



class OneBodyNode : public OneFrameNode
{
    Q_OBJECT
public:
    OneBodyNode(OpenSim::Body *body,NavigatorNode *parentNode,QObject *parent);
};

#endif // ONEBODYNODE_H
