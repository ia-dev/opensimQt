#ifndef NAVIGATORNODE_H
#define NAVIGATORNODE_H

#include <QObject>
#include <OpenSim/OpenSim.h>

class NavigatorNode : public QObject
{
    Q_OBJECT
public:
    explicit NavigatorNode(OpenSim::Object *_openSimObj,QString _displayName,NavigatorNode *_parentNode,QObject *parent = nullptr);


public:
    OpenSim::Object *openSimObject;
    QString displayName;
    NavigatorNode* parentNode;
    QList<NavigatorNode*> childNodes;
    QString iconPath;
signals:

};

#endif // NAVIGATORNODE_H
