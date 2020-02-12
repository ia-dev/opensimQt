#include "allforcesnode.h"
#include "musclesnode.h"
#include <QDebug>
AllForcesNode::AllForcesNode(OpenSim::Model *model,NavigatorNode *parentNode,QObject *parent):
    NavigatorNode(nullptr,"Forces",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/actuatorsNode.png";

    //loading Muscles
    model->updMuscles().setName("Muscles");
    MusclesNode *muscles = new MusclesNode(&model->updForceSet(),&model->updMuscles(),this,this);
    //  muscles groups display

    //loading actuators

}
