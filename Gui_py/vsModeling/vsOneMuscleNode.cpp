#include "vsOneMuscleNode.h"

vsOneMuscleNode::vsOneMuscleNode(OpenSim::Muscle *muscle,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(muscle,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/muscleNode.png";
}

void vsOneMuscleNode::setupNodeActions(QMenu *rootMenu)
{
    vsNavigatorNode::setupNodeActions(rootMenu);
    QAction *enableAction = new QAction("Enable",rootMenu);
    enableAction->setCheckable(true);
    enableAction->setChecked(true);
    rootMenu->addAction(enableAction);
}
