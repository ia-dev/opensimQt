#include "actuatorsnode.h"
#include "allforcesnode.h"
#include "contactforcesnode.h"
#include "musclesnode.h"
#include "otherforcesnode.h"
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
    ActuatorsNode *actuatorNode = new ActuatorsNode(&model->updForceSet(),this,this);

    //contact forces
    ContactForcesNode *contactForces = new ContactForcesNode(&model->updForceSet(),this,this);

    //other forces
    OtherForcesNode *otherForcesNode = new OtherForcesNode(&model->updForceSet(),this,this);


}
