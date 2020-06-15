/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#include "vsOneMotionNode.h"

vsOneMotionNode::vsOneMotionNode(OpenSim::Storage *motion ,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(motion,"",parentNode,parent),m_motion(motion)
{
    iconPath = ":/Data/Images/Nodes/motionNode.png";
    displayName = QString::fromStdString(motion->getName());
}
