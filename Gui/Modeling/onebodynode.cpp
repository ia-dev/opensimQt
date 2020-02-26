#include "onebodynode.h"
#include "onewrapobjectnode.h"

OneBodyNode::OneBodyNode(OpenSim::Body *body,NavigatorNode *parentNode,QObject *parent):
    OneFrameNode(body,parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/bodyNode.png";

    //wrap Objects
    OpenSim::WrapObjectSet wrapObjectSet = body->upd_WrapObjectSet();
    for (int j = 0; j < wrapObjectSet.getSize() ; ++j) {
        OpenSim::WrapObject *wrapObject = &wrapObjectSet.get(j);
        OneWrapObjectNode *wrapObjectNode = new OneWrapObjectNode(wrapObject,this,this);
    }
}
