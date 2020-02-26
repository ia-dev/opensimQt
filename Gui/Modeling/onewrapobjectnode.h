#ifndef ONEWRAPOBJECTNODE_H
#define ONEWRAPOBJECTNODE_H

#include "navigatornode.h"



class OneWrapObjectNode : public NavigatorNode
{
    Q_OBJECT
public:
    OneWrapObjectNode(OpenSim::WrapObject *wrapObj,NavigatorNode *parentNode,QObject *parent);
};

#endif // ONEWRAPOBJECTNODE_H
