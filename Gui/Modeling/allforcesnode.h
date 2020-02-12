#ifndef ALLFORCESNODE_H
#define ALLFORCESNODE_H

#include "navigatornode.h"



class AllForcesNode : public NavigatorNode
{
    Q_OBJECT
public:
    AllForcesNode(OpenSim::Model *model,NavigatorNode *parentNode,QObject *parent);

};

#endif // ALLFORCESNODE_H
