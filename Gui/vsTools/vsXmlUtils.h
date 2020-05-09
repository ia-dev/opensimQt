/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#ifndef VSXMLUTILS_H
#define VSXMLUTILS_H

#include <QObject>

class vsXmlUtils : public QObject
{
    Q_OBJECT
public:
    explicit vsXmlUtils(QObject *parent = nullptr);

    static void changeModelName(std::string modelFileName,std::string newName);

signals:

};

#endif // VSXMLUTILS_H
