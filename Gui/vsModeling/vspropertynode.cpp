/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#include "vsPropertyNode.h"

vsPropertyNode::vsPropertyNode(vsPropertyNode *_parentNode,QObject *parent) : QObject(parent),
    m_parentNode(_parentNode)
{
    if(m_parentNode != nullptr){
        m_parentNode->m_childNodes.append(this);
    }
}
