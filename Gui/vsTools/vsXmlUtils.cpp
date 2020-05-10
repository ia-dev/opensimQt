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
    XMLDocument *doc = new XMLDocument(String(modelFileName));
    auto modelElement = doc->getRootElement().getRequiredElement("Model");
    qDebug() <<"model tag/name :" << modelElement.getElementTag()
             << modelElement.getOptionalAttributeValue("name","name not fount") ;
    auto bodysetObjectsElement = modelElement.getRequiredElement("BodySet").getRequiredElement("objects");
    auto bodies  = bodysetObjectsElement.getAllElements("Body");
    for (int i = 0; i < bodies.size() ; ++i) {
        auto body = bodies.at(i);
        if(body.getRequiredAttributeValue("name")== currentName){
           body.setAttributeValue("name",newName);
            //TODO change the properties or reload the model
           auto elemList = findElementsWithContent(modelElement,"/bodyset/"+currentName);
           for(auto i = elemList.begin();i != elemList.end();i++){
               i->setValue("/bodyset/"+newName);
           }
           qDebug() << "element with reference"<< elemList.size();
           break;
        }
    }

    doc->writeToFile(modelFileName);

}

QList<Xml::Element> vsXmlUtils::findElementsWithContent(Xml::Element elem, std::string content)
{
    QList<Xml::Element> retList;
    //doc.fi
    if(elem.isValueElement() && (elem.getValue() == String(content))){
        retList << elem;
        qDebug() << elem.getElementTag()<< " " << elem.getValue();
    }

    for (auto i = elem.element_begin(); i != elem.element_end(); i++) {
        retList << findElementsWithContent(*i,content);
    }
    return retList;
}
