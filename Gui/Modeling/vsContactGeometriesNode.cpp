#include "vsContactGeometriesNode.h"
#include "vsOneContactGeometryNode.h"

vsContactGeometriesNode::vsContactGeometriesNode(OpenSim::ContactGeometrySet *contactGeometrySet,vsNavigatorNode *parentNavigator,QObject *parent):
    vsNavigatorNode(contactGeometrySet,"",parentNavigator,parent)
{
    iconPath = ":/Data/Images/Nodes/vsActuatorsNode.png";

    //loading contacts geometry
    for (int var = 0; var < contactGeometrySet->getSize(); ++var) {
        OpenSim::ContactGeometry *contactGeometry = &contactGeometrySet->get(var);

        vsOneContactGeometryNode *contactGeometryNode = new vsOneContactGeometryNode(contactGeometry,this,this);

    }
}
