#ifndef vsOneDofNode_H
#define vsOneDofNode_H

#include "vsNavigatorNode.h"



class vsOneDofNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsOneDofNode(OpenSim::TransformAxis *tAxis,vsNavigatorNode *parentNode,QObject *parent);

    // vsNavigatorNode interface
public:
    virtual void setupNodeActions(QMenu *rootMenu) override;
};

#endif // vsOneDofNode_H
