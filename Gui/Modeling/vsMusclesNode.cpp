#include "vsMuscleGroupeNode.h"
#include "vsMusclesNode.h"
#include "vsOneMuscleNode.h"
#include <QDebug>
vsMusclesNode::vsMusclesNode(OpenSim::ForceSet *forceSet,OpenSim::Set<OpenSim::Muscle> *muscleSet,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(&forceSet->updMuscles(),"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/vsMusclesNode.png";

    //loading muscles groupes
    OpenSim::Array<std::string> groups;
    forceSet->getGroupNames(groups);
    if(groups.getSize()>0)
    {
        for (int i = 0; i < groups.size(); ++i) {
            //qDebug() << QString::fromStdString(groups.get(i)
            OpenSim::ObjectGroup *muscleGroupe = const_cast<OpenSim::ObjectGroup*>(forceSet->getGroup(groups.get(i)));
            vsMuscleGroupeNode *muscleGroupeNode  = new vsMuscleGroupeNode(muscleGroupe,forceSet->updMuscles(),this,this);
        }
    }
    else
    {
        //loading muscles
        for (int i = 0; i < muscleSet->getSize(); ++i) {
            OpenSim::Muscle *muscle = &muscleSet->get(i);
            vsOneMuscleNode *muscleNode = new vsOneMuscleNode(muscle,this,this);
        }
    }

}
