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

class vsPropertyItem : public QStandardItem
{

public:
    enum PropertyType{
        Text,
        Check,
        List,
        Options,
        Color,
        Select
    };
    vsPropertyItem();

    QString m_name;
    QString m_value;
    PropertyType m_type;
};

#endif // VSPROPERTYITEM_H
