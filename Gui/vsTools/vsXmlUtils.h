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
#include <OpenSim.h>

using namespace OpenSim;
using namespace SimTK;

class vsXmlUtils : public QObject
{
    Q_OBJECT
public:
    explicit vsXmlUtils(QObject *parent = nullptr);

    static void changeModelName(std::string modelFileName,std::string newName);
    static void changeBodyName(std::string modelFileName,std::string currentName,std::string newName);
    static QList<Xml::Element> findElementsWithContent(Xml::Element elem,std::string content);


signals:

};

#endif // VSXMLUTILS_H
