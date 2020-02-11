#ifndef MODELNODE_H
#define MODELNODE_H

#include "navigatornode.h"
#include <QObject>


class ModelNode : public NavigatorNode
{
    Q_OBJECT
public:
    ModelNode(OpenSim::Model *model,NavigatorNode *parentNode,QObject *parent=nullptr);

};

#endif // MODELNODE_H
