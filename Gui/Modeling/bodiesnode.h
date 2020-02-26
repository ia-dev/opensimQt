#ifndef BODIESNODE_H
#define BODIESNODE_H

#include "navigatornode.h"



class BodiesNode : public NavigatorNode
{
    Q_OBJECT
public:
    BodiesNode(OpenSim::BodySet *bodySet,NavigatorNode *parentNode,QObject *parent);
};

#endif // BODIESNODE_H
