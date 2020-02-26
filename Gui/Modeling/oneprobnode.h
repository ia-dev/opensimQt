#ifndef ONEPROBNODE_H
#define ONEPROBNODE_H

#include "navigatornode.h"



class OneProbNode : public NavigatorNode
{
    Q_OBJECT
public:
    OneProbNode(OpenSim::Probe *probe,NavigatorNode *parentNode,QObject *parent);
};

#endif // ONEPROBNODE_H
