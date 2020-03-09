#ifndef vsContactGeometriesNode_H
#define vsContactGeometriesNode_H

#include "vsNavigatorNode.h"



class vsContactGeometriesNode : public vsNavigatorNode
{
    Q_OBJECT
public:
    vsContactGeometriesNode(OpenSim::ContactGeometrySet *contactGeometrySet,vsNavigatorNode *parentNavigator,QObject *parent);
};

#endif // vsContactGeometriesNode_H
