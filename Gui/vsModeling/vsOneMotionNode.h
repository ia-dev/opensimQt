/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#ifndef VSONEMOTIONNODE_H
#define VSONEMOTIONNODE_H

#include "vsNavigatorNode.h"

#include <QObject>

class vsOneMotionNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsOneMotionNode(OpenSim::Storage *motion ,vsNavigatorNode *parentNode,QObject *parent);
    OpenSim::Storage *m_motion;
    //TODO add the action for the motion node
};

#endif // VSONEMOTIONNODE_H
