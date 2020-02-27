#include "vsActuatorsNode.h"
#include "vsAllForcesNode.h"
#include "vsContactForcesNode.h"
#include "vsMusclesNode.h"
#include "vsOtherForcesNode.h"
#include <QDebug>
vsAllForcesNode::vsAllForcesNode(OpenSim::Model *model,vsNavigatorNode *parentNode,QObject *parent):
    vsNavigatorNode(nullptr,"Forces",parentNode,parent)
{
    iconPath = ":/Data/Images/Nodes/vsActuatorsNode.png";

    //loading Muscles
    model->updMuscles().setName("Muscles");
    vsMusclesNode *muscles = new vsMusclesNode(&model->updForceSet(),&model->updMuscles(),this,this);
    //  muscles groups display

    //loading actuators
    vsActuatorsNode *actuatorNode = new vsActuatorsNode(&model->updForceSet(),this,this);

    //contact forces
    vsContactForcesNode *contactForces = new vsContactForcesNode(&model->updForceSet(),this,this);

    //other forces
    vsOtherForcesNode *otherForcesNode = new vsOtherForcesNode(&model->updForceSet(),this,this);


}
