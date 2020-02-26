#include "bodiesnode.h"
#include "onebodynode.h"

BodiesNode::BodiesNode(OpenSim::BodySet *bodySet,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(bodySet,"Bodies",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/bodiesNode.png";

    //loading bodies
    for (int i = 0; i < bodySet->getSize(); i++) {
        OpenSim::Body *body = &(bodySet->get(i));
        OneBodyNode *bodyNode = new OneBodyNode(body,this,this);

    }
}
