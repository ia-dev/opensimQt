#ifndef NAVIGATORNODE_H
#define NAVIGATORNODE_H

#include <QObject>
#include <OpenSim/OpenSim.h>

class NavigatorNode : public QObject
{
    Q_OBJECT
public:
    explicit NavigatorNode(QObject *parent = nullptr);


public:
    OpenSim::Object *openSimObject;
    QString displayName;
    NavigatorNode* parentNode;
    QList<NavigatorNode*> childNodes;
signals:

};

#endif // NAVIGATORNODE_H
