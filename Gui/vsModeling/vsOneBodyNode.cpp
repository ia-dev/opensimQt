#include "vsOneBodyNode.h"
#include "vsOneWrapObjectNode.h"

vsOneBodyNode::vsOneBodyNode(OpenSim::Body *body,vsNavigatorNode *parentNode,QObject *parent):
    vsOneFrameNode(body,parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/bodyNode.png";

    //wrap Objects
    OpenSim::WrapObjectSet wrapObjectSet = body->upd_WrapObjectSet();
    for (int j = 0; j < wrapObjectSet.getSize() ; ++j) {
        OpenSim::WrapObject *wrapObject = &wrapObjectSet.get(j);
        vsOneWrapObjectNode *wrapObjectNode = new vsOneWrapObjectNode(wrapObject,this,this);
    }
}

void vsOneBodyNode::setupNodeActions(QMenu *rootMenu)
{
    vsOneFrameNode::setupNodeActions(rootMenu);
    QAction *showMassCenterAction = new QAction("Show Mass Center",rootMenu);
    QAction *renameAction = new QAction("Rename...",rootMenu);
    QAction *removeAction = new QAction("Remove",rootMenu);

    rootMenu->addActions(QList<QAction*>() << showMassCenterAction << renameAction << removeAction);

}
