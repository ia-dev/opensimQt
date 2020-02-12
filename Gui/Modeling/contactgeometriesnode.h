#ifndef CONTACTGEOMETRIESNODE_H
#define CONTACTGEOMETRIESNODE_H

#include "navigatornode.h"



class ContactGeometriesNode : public NavigatorNode
{
    Q_OBJECT
public:
    ContactGeometriesNode(OpenSim::ContactGeometrySet *contactGeometrySet,NavigatorNode *parentNavigator,QObject *parent);
};

#endif // CONTACTGEOMETRIESNODE_H
