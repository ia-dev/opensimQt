#ifndef vsProbesNode_H
#define vsProbesNode_H

#include "vsNavigatorNode.h"



class vsProbesNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsProbesNode(OpenSim::ProbeSet *probSet,vsNavigatorNode *parentNode,QObject *parent);

    // vsNavigatorNode interface
public:
    virtual void setupNodeActions(QMenu *rootMenu) override;
};

#endif // vsProbesNode_H
