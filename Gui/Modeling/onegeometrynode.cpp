#include "onegeometrynode.h"

#include <QDebug>

OneGeometryNode::OneGeometryNode(OpenSim::Geometry *geometry,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(geometry,"",parentNode,parent)

{
    iconPath = ":/Data/Images/Nodes/displayGeometry.png";

    //render the geometry


    //visualizerVTK->renderGeometry(geometry);
    OpenSim::PropertySet properySet = geometry->getPropertySet();
    //OpenSim::Mesh *mesh = OpenSim::Mesh::safeDownCast(geometry);
    //if(mesh != nullptr) mesh->set_mesh_file("F:\\FL\\3\\opensim-gui\\opensim-models\\Geometry\\"+mesh->get_mesh_file());

}
