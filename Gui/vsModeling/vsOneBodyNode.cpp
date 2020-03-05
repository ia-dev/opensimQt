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
