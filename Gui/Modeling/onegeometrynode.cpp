#include "onegeometrynode.h"

#include <QDebug>

OneGeometryNode::OneGeometryNode(OpenSim::Geometry *geometry,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(geometry,"",parentNode,parent)

{
    iconPath = ":/Data/Images/Nodes/displayGeometry.png";

    //render the geometry


    //visualizerVTK->renderGeometry(geometry);
    OpenSim::PropertySet properySet = geometry->getPropertySet();


}
