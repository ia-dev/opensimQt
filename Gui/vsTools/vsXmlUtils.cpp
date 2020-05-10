/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                        *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#include "vsXmlUtils.h"
#include <QDebug>


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
    //return modelElement;
}

void vsXmlUtils::changeBodyName(std::string modelFileName,std::string currentName,std::string newName)
{
    Xml::Document *doc = new Xml::Document(String(modelFileName));
    auto modelElement = doc->getRootElement().getRequiredElement("Model");
    qDebug() <<"model tag/name :" << modelElement.getElementTag()
             << modelElement.getOptionalAttributeValue("name","name not fount") ;
    auto bodysetObjectsElement = modelElement.getRequiredElement("BodySet").getRequiredElement("objects");
    auto bodies  = bodysetObjectsElement.getAllElements("Body");
    for (int i = 0; i < bodies.size() ; ++i) {
        auto body = bodies.at(i);
        if(body.getRequiredAttributeValue("name")== currentName){
           body.setAttributeValue("name",newName);

            //TODO search for the nodes using bodyset/oldname
        }
    }

    doc->writeToFile(modelFileName);

}
