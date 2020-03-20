#include "vsOneFrameNode.h"
#include "vsOneGeometryNode.h"

vsOneFrameNode::vsOneFrameNode(OpenSim::Frame *frame,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(frame,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/body.png";

    //geometry Objects
    for (int k = 0; k < frame->getProperty_attached_geometry().size(); ++k) {

        OpenSim::Geometry* geo = &frame->upd_attached_geometry(k);
        //qDebug()<< "first subcomponent > : "<< QString::fromStdString(geo->getName());
        vsOneGeometryNode *geometryObjectNode = new vsOneGeometryNode(geo,this,this);
    }
}

void vsOneFrameNode::setupNodeActions(QMenu *rootMenu)
{
    vsNavigatorNode::setupNodeActions(rootMenu);
    QAction *showAxisAction = new QAction("Show Axis",rootMenu);
    rootMenu->addAction(showAxisAction);
}
