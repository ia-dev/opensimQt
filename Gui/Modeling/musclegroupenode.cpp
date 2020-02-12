#include "musclegroupenode.h"
#include "onemusclenode.h"
#include <QDebug>
MuscleGroupeNode::MuscleGroupeNode(OpenSim::ObjectGroup *muscleGroup,OpenSim::Set<OpenSim::Muscle> muscles,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(nullptr,QString::fromStdString(muscleGroup->getName()),parentNode,parent)
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
//            OneMuscleNode *muscleNode= new OneMuscleNode(const_cast<OpenSim::Muscle*>(muscle),this,this);
////        } catch (...) {
////        }
//    }
    qDebug()<< "the size of the property "<< muscleGroup->getPropertyByName("members").size();
    for (int j = 0; j < muscleGroup->getPropertyByName("members").size(); ++j) {
         std::string muscleName = muscleGroup->getPropertyByName("members").getValue<std::string>(j);
         OpenSim::Muscle *muscle = &muscles.get(muscleName);
         OneMuscleNode *oneMuscleNode = new OneMuscleNode(muscle,this,this);
    }


}
