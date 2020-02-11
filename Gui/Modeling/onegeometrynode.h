#ifndef ONEGEOMETRYNODE_H
#define ONEGEOMETRYNODE_H

#include "navigatornode.h"



class OneGeometryNode : public NavigatorNode
{
    Q_OBJECT
public:
    OneGeometryNode(OpenSim::Geometry *geometry,NavigatorNode *parentNode,QObject *parent);
};

#endif // ONEGEOMETRYNODE_H
