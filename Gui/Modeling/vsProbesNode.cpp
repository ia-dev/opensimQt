#include "vsOneProbNode.h"
#include "vsProbesNode.h"

vsProbesNode::vsProbesNode(OpenSim::ProbeSet *probSet,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(probSet,"Probes",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/probe_multiple.png";
    for (int i = 0; i < probSet->getSize(); ++i) {
        OpenSim::Probe *prob = &probSet->get(i);
        vsOneProbNode *oneProbNode = new vsOneProbNode(prob,this,this);
    }
}
