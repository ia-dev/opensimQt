/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#include "vsObjectSetNode.h"

vsObjectSetNode::vsObjectSetNode(OpenSim::Object *objectSet,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(objectSet,"",parentNode,parent)
{

}

void vsObjectSetNode::setupNodeActions(QMenu *rootMenu)
{
    vsNavigatorNode::setupNodeActions(rootMenu);
    QStringList actionsToDisable;
    actionsToDisable << "Color..." <<"Opacity...";
    foreach (auto action, displayMenu->actions()) {
        if(actionsToDisable.contains(action->text())) action->setEnabled(false);
    }

}
