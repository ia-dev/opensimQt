#include "vsAllForcesNode.h"
#include "vsBodiesNode.h"
#include "vsConstraintsNode.h"
#include "vsContactGeometriesNode.h"
#include "vsControlersNode.h"
#include "vsGroundNode.h"
#include "vsJointsNode.h"
#include "vsMarkersNode.h"
#include "vsModelNode.h"
#include "vsProbesNode.h"

vsModelNode::vsModelNode(OpenSim::Model *model,vsNavigatorNode *parentNode,QObject *parent):vsNavigatorNode(model,"",parentNode,parent)
{

    iconPath = ":/Data/Images/Nodes/osimNode.png";

    //loading ground
    vsGroundNode *groundNode = new vsGroundNode(&model->updGround(),this,this);

    //loading bodySet
    vsBodiesNode *bodiesNode = new vsBodiesNode(&model->upd_BodySet(),this,this);

    //loading jointsSet
    vsJointsNode *jointsNode = new vsJointsNode(&model->updJointSet(),this,this);

    //loading constraint
    vsConstraintsNode *constraintNode = new vsConstraintsNode(&model->updConstraintSet(),this,this);

    //loading contact geometry
    vsContactGeometriesNode *contactGeometriesNode = new vsContactGeometriesNode(&model->updContactGeometrySet(),this,this);

    //loading forces
    vsAllForcesNode *allForcesNode = new vsAllForcesNode(model,this,this);

    //loading markers
    vsMarkersNode *markersNode = new vsMarkersNode(&model->updMarkerSet(),this,this);

    //loading controlers
    vsControlersNode *controlesNode =new vsControlersNode(&model->updControllerSet(),this,this);

    //loading probs
    vsProbesNode *probesNode = new vsProbesNode(&model->updProbeSet(),this,this);


    visualizerVTK->addOpenSimModel(model);


}
