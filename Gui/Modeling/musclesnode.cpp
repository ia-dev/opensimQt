#include "musclegroupenode.h"
#include "musclesnode.h"
#include "onemusclenode.h"
#include <QDebug>
MusclesNode::MusclesNode(OpenSim::ForceSet *forceSet,OpenSim::Set<OpenSim::Muscle> *muscleSet,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(&forceSet->updMuscles(),"",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/musclesNode.png";

    //loading muscles groupes
    OpenSim::Array<std::string> groups;
    forceSet->getGroupNames(groups);
    if(groups.getSize()>0)
    {
        for (int i = 0; i < groups.size(); ++i) {
            //qDebug() << QString::fromStdString(groups.get(i)
            OpenSim::ObjectGroup *muscleGroupe = const_cast<OpenSim::ObjectGroup*>(forceSet->getGroup(groups.get(i)));
            MuscleGroupeNode *muscleGroupeNode  = new MuscleGroupeNode(muscleGroupe,forceSet->updMuscles(),this,this);
        }
    }
    else
    {
        //loading muscles
        for (int i = 0; i < muscleSet->getSize(); ++i) {
            OpenSim::Muscle *muscle = &muscleSet->get(i);
            OneMuscleNode *muscleNode = new OneMuscleNode(muscle,this,this);
        }
    }

}
