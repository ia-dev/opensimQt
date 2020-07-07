/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#include "vsOneMotionNode.h"

#include "vsTools/vsMotionsUtils.h"

#include <QDebug>

vsOneMotionNode::vsOneMotionNode(OpenSim::Storage *motion,OpenSim::Model *model ,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(motion,"",parentNode,parent),m_motion(motion),m_model(model)
{
    iconPath = ":/Data/Images/Nodes/motionNode.png";
    displayName = QString::fromStdString(motion->getName());
}

void vsOneMotionNode::setupNodeActions(QMenu *rootMenu)
{
    //vsNavigatorNode::setupNodeActions(rootMenu);
    QAction *makeCurrentAction = new QAction("Make Current",rootMenu);
    QAction *renameAction = new QAction("Rename...",rootMenu);
    QAction *associateMotionDataAction = new QAction("Associate Motion Data...",rootMenu);
    QAction *colorMusclesFromFileAction = new QAction("Color Muscles From File...",rootMenu);
    QAction *syncMotionsAction = new QAction("Sync. Motions",rootMenu);
    QAction *saveAsAction = new QAction("Save As...",rootMenu);
    QAction *closeAction = new QAction("close",rootMenu);

    rootMenu->addActions(QList<QAction*>()<< makeCurrentAction << renameAction << associateMotionDataAction
                         << colorMusclesFromFileAction << syncMotionsAction << saveAsAction << closeAction);

    connect(makeCurrentAction,&QAction::triggered,this,&vsOneMotionNode::onMakeCurrentTriggered);
    //connect(makeCurrentAction,&QAction::triggered,[](){qDebug() << "motion made current";});
}

void vsOneMotionNode::onMakeCurrentTriggered()
{
    vsMotionsUtils::getInstance()->setCurrentMotion(m_model,m_motion);
}
