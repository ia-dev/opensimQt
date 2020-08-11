/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#ifndef VSOBJECTSETNODE_H
#define VSOBJECTSETNODE_H

#include "vsNavigatorNode.h"

#include <QObject>
#include <OpenSim.h>

class vsObjectSetNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsObjectSetNode(OpenSim::Object *objectSet,vsNavigatorNode *parentNode,QObject *parent);

    // vsNavigatorNode interface
public:
    virtual void setupNodeActions(QMenu *rootMenu) override;
};

#endif // VSOBJECTSETNODE_H
