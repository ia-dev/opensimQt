/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                          *
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
/**
 * The vsXmlUtils class contains helper functions to manipulate XML document such as osim files.
 * all the xml operations are used using the SimTK tinyXML library wich is the default one in opensim.
 */
class vsXmlUtils : public QObject
{
    Q_OBJECT
public:
    /// constructor. Note that all the operations for now are static, so no need to instanciate this class.
    explicit vsXmlUtils(QObject *parent = nullptr);

    /**
     * change the name of the model inside the given file.
     * @param modelFileName the file where the model is stored
     * @param newName the new name to replace the old one
     */
    static void changeModelName(std::string modelFileName,std::string newName);
    /**
     * change the name of the body with a name inside a model file (xml methode not openSim call).
     * @param modelFileName the path to the model file
     * @param currentName the current name of the desired body, to spot it
     * @param newName the new name of the body
     * @see findElementWithContent
     */
    static void changeBodyName(std::string modelFileName,std::string currentName,std::string newName);
    /**
     * find an offspring element of elem maching content
     * @param elem parent element
     * @param content the content criteria the returned element need to have
     * @return an XML::Element having content parameter as it content
     */
    static QList<Xml::Element> findElementsWithContent(Xml::Element elem,std::string content);


signals:

};

#endif // VSXMLUTILS_H
