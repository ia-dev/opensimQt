#include "vsBodiesNode.h"
#include "vsOneBodyNode.h"

vsBodiesNode::vsBodiesNode(OpenSim::BodySet *bodySet,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(bodySet,"Bodies",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/vsBodiesNode.png";

    //loading bodies
    for (int i = 0; i < bodySet->getSize(); i++) {
        OpenSim::Body *body = &(bodySet->get(i));
        vsOneBodyNode *bodyNode = new vsOneBodyNode(body,this,this);

    }
}
