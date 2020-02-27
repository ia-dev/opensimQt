#include "vsOneOtherForceNode.h"
#include "vsOtherForcesNode.h"
#include <QDebug>
vsOtherForcesNode::vsOtherForcesNode(OpenSim::ForceSet *forceSet,vsNavigatorNode *parentNode, QObject *parent):
    vsNavigatorNode(nullptr,"Other Forces",parentNode,parent)
{
   iconPath = ":/Data/Images/Nodes/forceNode.png";

   //loading Other forces
   for (int i = 0; i < forceSet->getSize(); ++i) {
       OpenSim::Force *force = &forceSet->get(i);

       OpenSim::Muscle *muscle = OpenSim::Muscle::safeDownCast(force);
       if(muscle != nullptr) continue;
       qDebug()<<"muscle concreate classnames "<< QString::fromStdString(force->getConcreteClassName())
              << QString::fromStdString(force->getConcreteClassName());

       if(QString::fromStdString(force->getConcreteClassName()) == "ElasticFoundationForce"||
               QString::fromStdString(force->getConcreteClassName()) == "HuntCrossleyForce") continue;

       vsOneOtherForceNode *contactForceNode = new vsOneOtherForceNode(force,this,this);

   }
}
