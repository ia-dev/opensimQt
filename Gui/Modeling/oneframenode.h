#ifndef ONEFRAMENODE_H
#define ONEFRAMENODE_H

#include "navigatornode.h"



class OneFrameNode : public NavigatorNode
{
    Q_OBJECT
public:
    OneFrameNode(OpenSim::Frame *frame,NavigatorNode *parentNode,QObject *parent);
};

#endif // ONEFRAMENODE_H
