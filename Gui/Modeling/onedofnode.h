#ifndef ONEDOFNODE_H
#define ONEDOFNODE_H

#include "navigatornode.h"



class OneDofNode : public NavigatorNode
{
    Q_OBJECT
public:
    OneDofNode(OpenSim::TransformAxis *tAxis,NavigatorNode *parentNode,QObject *parent);
};

#endif // ONEDOFNODE_H
