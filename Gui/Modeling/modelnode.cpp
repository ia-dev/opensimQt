#include "allforcesnode.h"
#include "bodiesnode.h"
#include "constraintsnode.h"
#include "contactgeometriesnode.h"
#include "controlersnode.h"
#include "groundnode.h"
#include "jointsnode.h"
#include "markersnode.h"
#include "modelnode.h"
#include "probesnode.h"

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

    //loading markers
    MarkersNode *markersNode = new MarkersNode(&model->updMarkerSet(),this,this);

    //loading controlers
    ControlersNode *controlesNode =new ControlersNode(&model->updControllerSet(),this,this);

    //loading probs
    ProbesNode *probesNode = new ProbesNode(&model->updProbeSet(),this,this);


    visualizerVTK->addOpenSimModel(model);

}
