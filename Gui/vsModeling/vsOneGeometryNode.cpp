#include "vsOneGeometryNode.h"

#include <QDebug>

vsOneGeometryNode::vsOneGeometryNode(OpenSim::Geometry *geometry,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(geometry,"",parentNode,parent)

{
    iconPath = ":/Data/Images/Nodes/displayGeometry.png";

    //render the geometry


    //visualizerVTK->renderGeometry(geometry);
    OpenSim::PropertySet properySet = geometry->getPropertySet();
    //OpenSim::Mesh *mesh = OpenSim::Mesh::safeDownCast(geometry);
    //if(mesh != nullptr) mesh->set_mesh_file("F:\\FL\\3\\opensim-gui\\opensim-models\\Geometry\\"+mesh->get_mesh_file());

}

void vsOneGeometryNode::setupNodeActions(QMenu *rootMenu)
{
    vsNavigatorNode::setupNodeActions(rootMenu);
    displayMenu->addSeparator();
    QAction *smoothShadedAction = new QAction("Smooth Shaded",displayMenu);
    QAction *wireFramAction = new QAction("Wireframe");
    displayMenu->addAction(smoothShadedAction);
    displayMenu->addAction(wireFramAction);
}
