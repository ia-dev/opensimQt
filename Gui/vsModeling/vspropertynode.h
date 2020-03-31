/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#ifndef VSPROPERTYNODE_H
#define VSPROPERTYNODE_H

#include <QObject>

class vsPropertyNode : public QObject
{
    Q_OBJECT
public:
    explicit vsPropertyNode(vsPropertyNode *_parentNode,QObject *parent = nullptr);

public:
    vsPropertyNode *m_parentNode;
    QList<vsPropertyNode*> m_childNodes;

signals:

};

#endif // VSPROPERTYNODE_H
