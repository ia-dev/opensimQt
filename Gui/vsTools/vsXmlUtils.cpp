/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#include "vsXmlUtils.h"
#include <OpenSim.h>
#include <QDebug>
using namespace OpenSim;
using namespace SimTK;

vsXmlUtils::vsXmlUtils(QObject *parent) : QObject(parent)
{

}

void vsXmlUtils::changeModelName(std::string modelFileName,std::string newName)
{
    Xml::Document *doc = new Xml::Document(String(modelFileName));
    auto modelElement = doc->getRootElement().getRequiredElement("Model");
    qDebug() <<"model tag/name :" << modelElement.getElementTag()
             << modelElement.getOptionalAttributeValue("name","name not fount") ;
    modelElement.getRequiredAttribute("name").setValue(newName);
    doc->writeToFile(modelFileName);

}
