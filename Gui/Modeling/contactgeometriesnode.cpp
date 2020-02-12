#include "contactgeometriesnode.h"
#include "onecontactgeometrynode.h"

ContactGeometriesNode::ContactGeometriesNode(OpenSim::ContactGeometrySet *contactGeometrySet,NavigatorNode *parentNavigator,QObject *parent):
    NavigatorNode(contactGeometrySet,"",parentNavigator,parent)
{
    iconPath = ":/Data/Images/Nodes/actuatorsNode.png";

    //loading contacts geometry
    for (int var = 0; var < contactGeometrySet->getSize(); ++var) {
        OpenSim::ContactGeometry *contactGeometry = &contactGeometrySet->get(var);

        OneContactGeometryNode *contactGeometryNode = new OneContactGeometryNode(contactGeometry,this,this);

    }
}
