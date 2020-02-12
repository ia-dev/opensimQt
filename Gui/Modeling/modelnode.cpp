#include "allforcesnode.h"
#include "bodiesnode.h"
#include "constraintsnode.h"
#include "contactgeometriesnode.h"
#include "groundnode.h"
#include "jointsnode.h"
#include "modelnode.h"

ModelNode::ModelNode(OpenSim::Model *model,NavigatorNode *parentNode,QObject *parent):NavigatorNode(model,"",parentNode,parent)
{

    iconPath = ":/Data/Images/Nodes/osimNode.png";

    //loading ground
    GroundNode *groundNode = new GroundNode(&model->updGround(),this,this);

    //loading bodySet
    BodiesNode *bodiesNode = new BodiesNode(&model->upd_BodySet(),this,this);

    //loading jointsSet
    JointsNode *jointsNode = new JointsNode(&model->updJointSet(),this,this);

    //loading constraint
    ConstraintsNode *constraintNode = new ConstraintsNode(&model->updConstraintSet(),this,this);

    //loading contact geometry
    ContactGeometriesNode *contactGeometriesNode = new ContactGeometriesNode(&model->updContactGeometrySet(),this,this);

    //loading forces

    AllForcesNode *allForcesNode = new AllForcesNode(model,this,this);



}
