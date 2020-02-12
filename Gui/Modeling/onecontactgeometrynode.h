#ifndef ONECONTACTGEOMETRYNODE_H
#define ONECONTACTGEOMETRYNODE_H

#include "navigatornode.h"



class OneContactGeometryNode : public NavigatorNode
{
    Q_OBJECT
public:
    OneContactGeometryNode(OpenSim::ContactGeometry *contactGeometry,NavigatorNode *parentNode,QObject *parent);
};

#endif // ONECONTACTGEOMETRYNODE_H
