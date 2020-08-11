#ifndef vsOneBodyNode_H
#define vsOneBodyNode_H

#include "vsOneFrameNode.h"



class vsOneBodyNode : public vsOneFrameNode
{
    Q_OBJECT
public:
    vsOneBodyNode(OpenSim::Body *body,vsNavigatorNode *parentNode,QObject *parent);

    // vsNavigatorNode interface
public:
    virtual void setupNodeActions(QMenu *rootMenu) override;

public slots:
    void onRenameActionTriggered();
};

#endif // vsOneBodyNode_H
