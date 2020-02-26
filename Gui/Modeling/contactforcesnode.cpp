#include "contactforcesnode.h"
#include "onecontactforcenode.h"
#include <QDebug>
ContactForcesNode::ContactForcesNode(OpenSim::ForceSet *forceSet,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(nullptr,"Contact Forces",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/forceNode.png";
    for (int i = 0; i < forceSet->getSize(); ++i) {
        OpenSim::Force *force = &forceSet->get(i);

        qDebug()<<"muscle concreate classnames "<< QString::fromStdString(force->getConcreteClassName())
               << QString::fromStdString(force->getConcreteClassName());

        if(QString::fromStdString(force->getConcreteClassName()) == "ElasticFoundationForce"||
                QString::fromStdString(force->getConcreteClassName()) == "HuntCrossleyForce")
        {
            OneContactForceNode *contactForceNode = new OneContactForceNode(force,this,this);
        }
    }

}
