#include "oneframenode.h"
#include "onegeometrynode.h"

OneFrameNode::OneFrameNode(OpenSim::Frame *frame,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(frame,"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/body.png";

    //geometry Objects
    for (int k = 0; k < frame->getProperty_attached_geometry().size(); ++k) {

        OpenSim::Geometry* geo = &frame->upd_attached_geometry(k);
        //qDebug()<< "first subcomponent > : "<< QString::fromStdString(geo->getName());
        OneGeometryNode *geometryObjectNode = new OneGeometryNode(geo,this,this);
    }
}
