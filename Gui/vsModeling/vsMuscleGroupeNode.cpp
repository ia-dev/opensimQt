#include "vsMuscleGroupeNode.h"
#include "vsOneMuscleNode.h"
#include <QDebug>
vsMuscleGroupeNode::vsMuscleGroupeNode(OpenSim::ObjectGroup *muscleGroup,OpenSim::Set<OpenSim::Muscle> muscles,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(nullptr,QString::fromStdString(muscleGroup->getName()),parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/muscleNode.png";
//    //loading muscles
//    OpenSim::Array<const OpenSim::Object*> members = muscleGroup->getPropertyByName("members");
//    qDebug()<< "the size of the group 1" << members.getSize();
//    foreach (const OpenSim::Object *obj , members) {
//        qDebug()<< "name " << QString::fromStdString(obj->getName());
//    }
//    for (int i = 0; i < muscleGroup->getMembers().getSize(); ++i) {
//        //try {
//            qDebug()<<"the numbers are here "<< i;
//            const OpenSim::Muscle *muscle  =static_cast<const OpenSim::Muscle*>(muscleGroup->getMembers().get(i));
//            vsOneMuscleNode *muscleNode= new vsOneMuscleNode(const_cast<OpenSim::Muscle*>(muscle),this,this);
////        } catch (...) {
////        }
//    }
    qDebug()<< "the size of the property "<< muscleGroup->getPropertyByName("members").size();
    for (int j = 0; j < muscleGroup->getPropertyByName("members").size(); ++j) {
         std::string muscleName = muscleGroup->getPropertyByName("members").getValue<std::string>(j);
         OpenSim::Muscle *muscle = &muscles.get(muscleName);
         vsOneMuscleNode *oneMuscleNode = new vsOneMuscleNode(muscle,this,this);
    }


}
