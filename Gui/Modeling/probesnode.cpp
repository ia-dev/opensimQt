#include "oneprobnode.h"
#include "probesnode.h"

ProbesNode::ProbesNode(OpenSim::ProbeSet *probSet,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(probSet,"Probes",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/probe_multiple.png";
    for (int i = 0; i < probSet->getSize(); ++i) {
        OpenSim::Probe *prob = &probSet->get(i);
        OneProbNode *oneProbNode = new OneProbNode(prob,this,this);
    }
}
