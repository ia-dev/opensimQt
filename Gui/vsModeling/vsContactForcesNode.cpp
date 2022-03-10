#include "vsContactForcesNode.h"
#include "vsOneContactForceNode.h"
#include <QDebug>
vsContactForcesNode::vsContactForcesNode(OpenSim::ForceSet *forceSet,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(nullptr,"Contact Forces",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/forceNode.png";
    editColorAndOpacity = false;
    for (int i = 0; i < forceSet->getSize(); ++i) {
        OpenSim::Force *force = &forceSet->get(i);

        qDebug()<<"muscle concreate classnames "<< QString::fromStdString(force->getConcreteClassName())
               << QString::fromStdString(force->getConcreteClassName());

        if(QString::fromStdString(force->getConcreteClassName()) == "ElasticFoundationForce"||
                QString::fromStdString(force->getConcreteClassName()) == "HuntCrossleyForce")
        {
            vsOneContactForceNode *contactForceNode = new vsOneContactForceNode(force,this,this);
        }
    }

}
