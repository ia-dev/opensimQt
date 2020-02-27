#ifndef vsModelNode_H
#define vsModelNode_H

#include "vsNavigatorNode.h"
#include <QObject>


class vsModelNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsModelNode(OpenSim::Model *model,vsNavigatorNode *parentNode,QObject *parent=nullptr);

};

#endif // vsModelNode_H
