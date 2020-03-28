#ifndef vsModelNode_H
#define vsModelNode_H

#include "vsNavigatorNode.h"
#include <QObject>


class vsModelNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsModelNode(OpenSim::Model *model,vsNavigatorNode *parentNode,QObject *parent=nullptr);

public slots:
    void onCloseModelClicked();

    // vsNavigatorNode interface
public:
    virtual void setupNodeActions(QMenu *rootMenu) override;
};

#endif // vsModelNode_H
