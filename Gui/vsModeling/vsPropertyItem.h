/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#ifndef VSPROPERTYITEM_H
#define VSPROPERTYITEM_H

#include <QObject>
#include <QStandardItem>
#include <OpenSim.h>

class vsPropertyItem : public QStandardItem
{

public:
    enum PropertyType{
        Text,
        TextObj,
        Check,
        List,
        Options,
        Color,
        Object,
        GeometryPath,
        Function
    };
    vsPropertyItem();

    QString m_name;
    QString m_value;
    PropertyType m_type;
    bool m_isEditable;
    OpenSim::Object* m_object;
};

#endif // VSPROPERTYITEM_H
